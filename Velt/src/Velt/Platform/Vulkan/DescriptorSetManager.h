#pragma once
#include "Core/Core.h"
#include <vulkan/vulkan.h>

namespace Velt::RHI
{ 
    class DescriptorSetManager
    {
    public:
        DescriptorSetManager() {};
        virtual ~DescriptorSetManager() {};

        void Init();
        void Shutdown(); 

        VkDescriptorSet Allocate(VkDescriptorSetLayout layout, u32 maxSetsHint = 128);
        private: 
        void GrapPool(); 

        private:
        VkDescriptorPool m_CurrentPool;
        
        std::vector<VkDescriptorPool> m_UsedPools;

    }
}