#include "vtpch.h"
#include "Core/Core.h"
#include "VulkanResourceUploader.h"
#include "VulkanContext.h"

namespace Velt::Renderer::Vulkan
{
    void VulkanResourceUploader::Begin()
    {
        
        vkWaitForFences(
            m_Device.device(),
            1,
            &m_Fence,
            VK_TRUE,
            UINT64_MAX
        );

        vkResetFences(m_Device.device(), 1, &m_Fence);

        // Command Buffer zurücksetzen
        vkResetCommandBuffer(m_Commandbuffer, 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(m_Commandbuffer, &beginInfo);
    }

    void VulkanResourceUploader::End()
    {
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

        // Warten bis Upload fertig
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