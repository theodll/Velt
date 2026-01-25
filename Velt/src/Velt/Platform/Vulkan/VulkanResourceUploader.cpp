#include "vtpch.h"
#include "Core/Core.h"
#include "VulkanResourceUploader.h"
#include "VulkanContext.h"

namespace Velt::Renderer::RHI
{
    void VulkanResourceUploader::Begin()
    {
        VT_PROFILE_SCOPE("VulkanResourceUploader Begin");
        VT_CORE_TRACE("Begin Vulkan Resource Uploader");

        if (vkWaitForFences(
            m_Device.device(),
            1,
            &m_Fence,
            VK_TRUE,
            UINT64_MAX
        ) != VK_SUCCESS) 
        {
            VT_CORE_ASSERT(false, "Failed to wait for Fences");
        }

        vkResetFences(m_Device.device(), 1, &m_Fence);

        vkResetCommandBuffer(m_Commandbuffer, 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(m_Commandbuffer, &beginInfo);
        
    }

    void VulkanResourceUploader::End()
    {
        VT_PROFILE_SCOPE("VulkanResourceUploader End");
        VT_CORE_TRACE("End Vulkan Resource Uploader");

        vkEndCommandBuffer(m_Commandbuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_Commandbuffer;

        vkQueueSubmit(
            m_Device.graphicsQueue(),
            1,
            &submitInfo,
            m_Fence
        );

        vkWaitForFences(
            m_Device.device(),
            1,
            &m_Fence,
            VK_TRUE,
            UINT64_MAX
        );
        }

    void VulkanResourceUploader::Init()
    {
        VT_PROFILE_FUNCTION();
        VT_CORE_TRACE("Init Vulkan Resource Uploader");
        u32 graphicsQueueFamily = m_Device.GetQueueFamilyIndex();

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = graphicsQueueFamily;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(
                m_Device.device(),
                &poolInfo,
                nullptr,
                &m_CommandPool) != VK_SUCCESS)
        {
            VT_CORE_ASSERT(false, "Failed to create Commandpool.")
        }

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_CommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(
                m_Device.device(),
                &allocInfo,
                &m_Commandbuffer) != VK_SUCCESS)
        {
            VT_CORE_ASSERT(false, "Failed to Allocate Commandbuffers");
        };

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateFence(
            m_Device.device(),
            &fenceInfo,
            nullptr,
            &m_Fence) != VK_SUCCESS) 
        {
            VT_CORE_ASSERT(false, "Failed to create Fence");
        };
    }

    void VulkanResourceUploader::Shutdown() 
    {
        VT_PROFILE_FUNCTION();
        VT_CORE_TRACE("Shutdown Vulkan Resource Uploader");
        if (m_Fence)
            vkDestroyFence(m_Device.device(), m_Fence, nullptr);

        if (m_CommandPool)
            vkDestroyCommandPool(m_Device.device(), m_CommandPool, nullptr);

        m_Fence = VK_NULL_HANDLE;
        m_CommandPool = VK_NULL_HANDLE;
        m_Commandbuffer = VK_NULL_HANDLE;
    }

    VulkanResourceUploader::VulkanResourceUploader() : m_Device(VulkanContext::GetDevice())
    {
    }

}