#pragma once
#include "Core/Core.h"
#include "Core/Application.h"
#include "Renderer/Swapchain.h"
#include "VulkanDevice.h"
#include <vulkan/vulkan.h>

namespace Velt::RHI
{ 
	struct PoolSizes {
        std::vector<std::pair<VkDescriptorType, float>> sizes = {
            { VK_DESCRIPTOR_TYPE_SAMPLER,                0.5f },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.0f },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          4.0f },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1.0f },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1.0f },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1.0f },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         2.0f },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         2.0f },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.0f },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.0f },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       0.5f }
        };
    };

    struct FDAllocator
    {
        // Todo [19.02.26, Theo]: Change the 3 to a getter to get the MaxFramesInFlight

        // Note [19.02.26, Theo]: The 3 stands for the MaxFramesInFlight

        DescriptorSetManager allocs[3];

        void Init()
        {
            for (auto& a : allocs) a.Init();
        }

        void BeginFrame(u32 frameIndex)
        {
            allocs[frameIndex].ResetPools();
        }

        VkDescriptorSet Allocate(u32 frameIndex, VkDescriptorSetLayout layout)
        {
            return allocs[frameIndex].Allocate(layout, frameIndex); 
        }

    };

    class DescriptorSetManager
    {
    public:
        DescriptorSetManager() : m_Device(VulkanContext::GetDevice()) {};
        virtual ~DescriptorSetManager() {};

        void Init();
        void Shutdown(); 

        // Note [16.02.26, Theo]: ResetPools is only ment for persistent Pools. Do NOT reset every frame ! 
        // Only use when you know you are not going to use the sets anymore

        void ResetPools();
        void SetPoolSize(const PoolSizes& sizes) { m_PoolSizes = std::move(sizes); }; 

        VkDescriptorSet Allocate(VkDescriptorSetLayout layout, u32 maxSetsHint = 128);

        private: 
    	VkDescriptorPool GrabPool(u32 maxSets);
        VkDescriptorPool CreatePool(u32 maxSets);

        private:
        VulkanDevice& m_Device; 
        VkDescriptorPool m_CurrentPool = VK_NULL_HANDLE;
        
        std::vector<VkDescriptorPool> m_UsedPools;
        std::vector<VkDescriptorPool> m_FreePools; 

        PoolSizes m_PoolSizes{}; 
    };
}