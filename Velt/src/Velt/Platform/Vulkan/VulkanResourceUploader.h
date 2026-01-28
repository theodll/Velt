#pragma once
#include <vulkan/vulkan.h>
#include "VulkanDevice.h"

namespace Velt::Renderer::RHI  
{

    class VulkanResourceUploader 
    {
        public: 

        VulkanResourceUploader();
        void Init();
        void Shutdown();

        void Begin(); 
        void End();

        VkCommandBuffer GetCommandBuffer() const { return m_Commandbuffer; };
        
        private:
        VkCommandPool m_CommandPool;
        VkCommandBuffer m_Commandbuffer;
        VkFence m_Fence;
        VulkanDevice& m_Device;
    };

}