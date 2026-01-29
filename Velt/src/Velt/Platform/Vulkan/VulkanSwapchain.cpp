#include "vtpch.h"
#include "VulkanSwapchain.h"
#include "VulkanContext.h"
#include <SDL3/SDL_vulkan.h>
#include <SDL3/SDL.h>
#include "Core/Application.h"

namespace Velt::Renderer::RHI
{

    void VulkanSwapchain::Init(SwapchainCreateInfo& createInfo)
    {
        VT_PROFILE_FUNCTION();
        VT_CORE_TRACE("Swapchain Created");
        auto&& window = (SDL_Window*)Velt::Application::Get().GetWindow().GetNativeHandle();
        i32 w{}, h{};
        SDL_GetWindowSizeInPixels(window, &w, &h);
        m_WindowExtent = { (u32)w, (u32)h };

        Create(createInfo);
        m_Instance = VulkanContext::GetInstance();

        VT_CORE_ERROR("Swapchain Size: {0}, {1}", GetWidth(), GetHeight());
        VT_CORE_ERROR("Swapchain Extend: {0}, {1}", m_WindowExtent.width, m_WindowExtent.height);
    }

    void VulkanSwapchain::Destroy()
    {
        VT_PROFILE_FUNCTION();

        if (m_Device.device() == nullptr)
        {
            VT_CORE_ERROR("Device is null");
        }

        vkDeviceWaitIdle(m_Device.device());

        for (auto image : m_SwapchainImages)
        {
            vkDestroyImageView(m_Device.device(), image.ImageView, nullptr);
        }
        m_SwapchainImages.clear();

        if (m_Swapchain != nullptr)
        {
            vkDestroySwapchainKHR(m_Device.device(), m_Swapchain, nullptr);
            m_Swapchain = nullptr;
        }

        for (auto depthStencilImage : m_DepthStencilImages)
        {
            vkDestroyImageView(m_Device.device(), depthStencilImage.DepthImageView, nullptr);
            vkDestroyImage(m_Device.device(), depthStencilImage.DepthImage, nullptr);
            vkFreeMemory(m_Device.device(), depthStencilImage.DepthImageMemory, nullptr);
        }

        for (size_t i = 0; i < m_ImageAvailableSemaphores.size(); i++)
        {
            vkDestroySemaphore(m_Device.device(), m_RenderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(m_Device.device(), m_ImageAvailableSemaphores[i], nullptr);
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            vkDestroyFence(m_Device.device(), m_InFlightFences[i], nullptr);
        }

        for (auto& frame : m_Commandbuffers)
        {
            vkDestroyCommandPool(m_Device.device(), frame.CommandPool, nullptr);
        }

        m_Commandbuffers.clear();
    }

    u32 VulkanSwapchain::AcquireNextImage()
    {
        VT_PROFILE_FUNCTION();
        vkWaitForFences(
            m_Device.device(),
            1,
            &m_InFlightFences[m_CurrentFrameIndex],
            VK_TRUE,
            std::numeric_limits<uint64_t>::max());

        VkResult result = vkAcquireNextImageKHR(
            m_Device.device(),
            m_Swapchain,
            std::numeric_limits<uint64_t>::max(),
            m_ImageAvailableSemaphores[m_CurrentFrameIndex],
            VK_NULL_HANDLE,
            &m_CurrentImageIndex);

        return result;
    }

    u32 VulkanSwapchain::SubmitCommandBuffers(
        const VkCommandBuffer* buffers, u32* imageIndex)
    {
        VT_PROFILE_FUNCTION();
        if (m_ImagesInFlight[*imageIndex] != VK_NULL_HANDLE)
        {
            vkWaitForFences(m_Device.device(), 1, &m_ImagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
        }
        m_ImagesInFlight[*imageIndex] = m_InFlightFences[m_CurrentFrameIndex];
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrameIndex] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = buffers;

        VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrameIndex] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(m_Device.device(), 1, &m_InFlightFences[m_CurrentFrameIndex]);
        if (vkQueueSubmit(m_Device.graphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrameIndex]) != VK_SUCCESS)
        {
            VT_CORE_ASSERT(false, "Failed to Submit Draw Commandbuffers")
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_Swapchain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = imageIndex;

        auto result = vkQueuePresentKHR(m_Device.presentQueue(), &presentInfo);

        // Mark this image as having been presented at least once
        m_ImagePresentedOnce[*imageIndex] = true;

        vkQueueWaitIdle(m_Device.presentQueue());

        m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
    }

    void VulkanSwapchain::Create(SwapchainCreateInfo& createInfo)
    {
        VT_PROFILE_FUNCTION();
        VT_PROFILE_FUNCTION();
        SwapChainSupportDetails swapChainSupport = m_Device.GetSwapChainSupport();

        VT_CORE_ERROR("Swapchain Creation Info Extend: {0}, {1}", createInfo.Width, createInfo.Height);
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        VT_CORE_ERROR("Chosen Swapchain Extend: {0}, {1}", extent.width, extent.height);
        

        u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfoVk = {};
        createInfoVk.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfoVk.surface = m_Device.surface();

        createInfoVk.minImageCount = imageCount;
        createInfoVk.imageFormat = surfaceFormat.format;
        createInfoVk.imageColorSpace = surfaceFormat.colorSpace;
        createInfoVk.imageExtent = extent;
        createInfoVk.imageArrayLayers = 1;
        createInfoVk.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = m_Device.FindPhysicalQueueFamilies();
        u32 queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfoVk.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfoVk.queueFamilyIndexCount = 2;
            createInfoVk.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfoVk.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfoVk.queueFamilyIndexCount = 0;
            createInfoVk.pQueueFamilyIndices = nullptr;
        }

        createInfoVk.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfoVk.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfoVk.presentMode = presentMode;
        createInfoVk.clipped = VK_TRUE;

        createInfoVk.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_Device.device(), &createInfoVk, nullptr, &m_Swapchain) != VK_SUCCESS)
        {
            VT_CORE_ERROR("Failed to create swapchain!");
        }

        vkGetSwapchainImagesKHR(m_Device.device(), m_Swapchain, &imageCount, nullptr);
        m_SwapchainImages.resize(imageCount);

        std::vector<VkImage> images(imageCount);
        vkGetSwapchainImagesKHR(m_Device.device(), m_Swapchain, &imageCount, images.data());

        for (size_t i = 0; i < imageCount; i++)
        {
            m_SwapchainImages[i].Image = images[i];
        }

        m_SwapChainImageFormat = surfaceFormat.format;
        m_WindowExtent = extent;

        for (size_t i = 0; i < m_SwapchainImages.size(); i++)
        {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_SwapchainImages[i].Image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = m_SwapChainImageFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_Device.device(), &viewInfo, nullptr, &m_SwapchainImages[i].ImageView) !=
                VK_SUCCESS)
            {
                VT_CORE_ASSERT(false, "Failed to create Image View.");
            }
        }

        // Create depth resources
        VkFormat depthFormat = findDepthFormat();
        VkExtent2D swapChainExtent = m_WindowExtent;

        m_DepthStencilImages.resize(imageCount);

        for (int i = 0; i < m_DepthStencilImages.size(); i++)
        {
            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = swapChainExtent.width;
            imageInfo.extent.height = swapChainExtent.height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.format = depthFormat;
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.flags = 0;

            m_Device.createImageWithInfo(
                imageInfo,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                m_DepthStencilImages[i].DepthImage,
                m_DepthStencilImages[i].DepthImageMemory);

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_DepthStencilImages[i].DepthImage;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = depthFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_Device.device(), &viewInfo, nullptr, &m_DepthStencilImages[i].DepthImageView) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create texture image view!");
            }
        }

        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        m_ImagesInFlight.resize(imageCount, VK_NULL_HANDLE);
        m_ImagePresentedOnce.resize(imageCount, false); // Initialize all to false

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        // Create semaphores for each frame in flight
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(m_Device.device(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) !=
                VK_SUCCESS ||
                vkCreateSemaphore(m_Device.device(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) !=
                VK_SUCCESS ||
                vkCreateFence(m_Device.device(), &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
            {
                VT_CORE_ASSERT(false, "Failed to Create Synchronisation Primitives");
            }
        }

        m_Commandbuffers.resize(MAX_FRAMES_IN_FLIGHT);

        for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            VkCommandPoolCreateInfo poolInfo{};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.queueFamilyIndex = m_Device.GetQueueFamilyIndex();
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

            vkCreateCommandPool(
                m_Device.device(),
                &poolInfo,
                nullptr,
                &m_Commandbuffers[i].CommandPool
            );

            VkCommandBufferAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = m_Commandbuffers[i].CommandPool;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = 1;

            vkAllocateCommandBuffers(
                m_Device.device(),
                &allocInfo,
                &m_Commandbuffers[i].CommandBuffer
            );
        }
    }

    void VulkanSwapchain::BeginFrame() 
    {
        AcquireNextImage();
    
    }

    VkSurfaceFormatKHR VulkanSwapchain::chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        VT_PROFILE_FUNCTION();
        for (const auto& availableFormat : availableFormats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapchain::chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        VT_PROFILE_FUNCTION();
        /*for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                VT_CORE_INFO("Present mode: Mailbox");
                return availablePresentMode;
            }
        }*/

        VT_CORE_INFO("Present mode: V-Sync");
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        VT_PROFILE_FUNCTION();

        if (capabilities.currentExtent.width != std::numeric_limits<u32>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            VkExtent2D actualExtent = m_WindowExtent;
            actualExtent.width = std::max(
                capabilities.minImageExtent.width,
                std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(
                capabilities.minImageExtent.height,
                std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    VkFormat VulkanSwapchain::findDepthFormat()
    {
        VT_PROFILE_FUNCTION();
        return m_Device.FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

	void VulkanSwapchain::Present()
	{
		VT_PROFILE_FUNCTION();

		u32 imageIndex = m_CurrentImageIndex;
        VkCommandBuffer commandBuffer = GetDrawCommandBuffer(m_CurrentFrameIndex);

		VkCommandBuffer commandBuffers[] = { commandBuffer };

		SubmitCommandBuffers(commandBuffers, &imageIndex);
	}

    void VulkanSwapchain::InitSurface(SDL_Window* windowHandle)
    {
        VkPhysicalDevice physicalDevice = m_Device.GetPhysicalDevice();

        u32 queueCount;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);
        VT_CORE_ASSERT(queueCount >= 1, "No queue families found!");

        std::vector<VkQueueFamilyProperties> queueProps(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueProps.data());

        std::vector<VkBool32> supportsPresent(queueCount);
        for (uint32_t i = 0; i < queueCount; i++)
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_Surface, &supportsPresent[i]);
        }

        u32 graphicsQueueNodeIndex = UINT32_MAX;
        u32 presentQueueNodeIndex = UINT32_MAX;

        for (u32 i = 0; i < queueCount; i++)
        {
            if ((queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
            {
                if (graphicsQueueNodeIndex == UINT32_MAX)
                {
                    graphicsQueueNodeIndex = i;
                }

                if (supportsPresent[i] == VK_TRUE)
                {
                    graphicsQueueNodeIndex = i;
                    presentQueueNodeIndex = i;
                    break;
                }
            }
        }

        if (presentQueueNodeIndex == UINT32_MAX)
        {
            for (u32 i = 0; i < queueCount; ++i)
            {
                if (supportsPresent[i] == VK_TRUE)
                {
                    presentQueueNodeIndex = i;
                    break;
                }
            }
        }

        VT_CORE_ASSERT(graphicsQueueNodeIndex != UINT32_MAX, "No graphics queue family found!");
        VT_CORE_ASSERT(presentQueueNodeIndex != UINT32_MAX, "No present queue family found!");

        m_QueueNodeIndex = graphicsQueueNodeIndex;

        FindImageFormatAndColorSpace();
    }


	// Copyright (c) 2023 TheCherno
	// Licensed under the Apache License 2.0

    void VulkanSwapchain::FindImageFormatAndColorSpace()
    {
        VkPhysicalDevice physicalDevice = m_Device.GetPhysicalDevice();

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, NULL);
        VT_CORE_ASSERT(formatCount > 0, "No surface formats found.");

        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, surfaceFormats.data());

        if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
        {
            m_ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
            m_ColorSpace = surfaceFormats[0].colorSpace;
        }
        else
        {
            bool found_B8G8R8A8_UNORM = false;
            for (auto&& surfaceFormat : surfaceFormats)
            {
                if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
                {
                    m_ColorFormat = surfaceFormat.format;
                    m_ColorSpace = surfaceFormat.colorSpace;
                    found_B8G8R8A8_UNORM = true;
                    break;
                }
            }

            if (!found_B8G8R8A8_UNORM)
            {
                m_ColorFormat = surfaceFormats[0].format;
                m_ColorSpace = surfaceFormats[0].colorSpace;
            }
        }

    }

	void VulkanSwapchain::TransitionImageLayout(
		VkCommandBuffer commandBuffer,
		VkImage image,
		VkImageLayout oldLayout,
		VkImageLayout newLayout,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask)
	{
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL &&
			newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		{
			barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			barrier.dstAccessMask = 0;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
			newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR &&
			newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL &&
                newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        }
        else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
                newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
        {
            barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        }
        else
        {
            VT_CORE_ASSERT(false, "Unsupported layout transition");
        }

		vkCmdPipelineBarrier(
			commandBuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);
	}

    VulkanSwapchain::VulkanSwapchain() : m_Device(VulkanContext::GetDevice()), m_Instance(VulkanContext::GetInstance()), m_Surface(VulkanContext::GetSurface())
    {

    }

}