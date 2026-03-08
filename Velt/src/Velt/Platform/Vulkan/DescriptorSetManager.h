#pragma once
#include "Core/Core.h"
#include "Renderer/Swapchain.h"
#include "VulkanDevice.h"
#include "VulkanContext.h"
#include <vulkan/vulkan.h>

namespace Velt::RHI
{ 
    class Application;

    static const char* VkResultToString(VkResult r)
    {
        switch (r)
        {
        case VK_SUCCESS: return "VK_SUCCESS";
        case VK_NOT_READY: return "VK_NOT_READY";
        case VK_TIMEOUT: return "VK_TIMEOUT";
        case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST";
        default: return "UNKNOWN_VK_RESULT";
        }
    }

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

    class DescriptorSetManager
    {
    public:
        DescriptorSetManager() : m_Device(*VulkanContext::GetDevice()) {};
        virtual ~DescriptorSetManager() {};

        void Init();
        void Shutdown(); 

        // Note [16.02.26, Theo]: ResetPools is only ment for persistent Pools. Do NOT reset every frame ! 
        // Only use when you know you are not going to use the sets anymore

        void ResetPools();
        void SetPoolSize(const PoolSizes& sizes) { m_PoolSizes = std::move(sizes); }; 
        void WriteBuffer(VkDescriptorSet set, uint32_t binding, VkBuffer buffer, VkDeviceSize size);
        void WriteImage(VkDescriptorSet set, u32 binding, const VkDescriptorImageInfo& info);


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
}