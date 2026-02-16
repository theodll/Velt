#include "vtpch.h"
#include "Core/Core.h"
#include "DescriptorSetManager.h"

namespace Velt::RHI 
{
    void DescriptorSetManager::ResetPools() 
    {
        for (auto p : m_UsedPools) 
        {
            vkResetDescriptorPool(m_Device.device(), p, 0);
            m_FreePools.emplace_back(p);
        };
        m_UsedPools.clear(); 
        m_CurrentPool = VK_NULL_HANDLE; 
    }

    VkDescriptorSet DescriptorSetManager::Allocate(VkDescriptorSetLayout layout, u32 maxSetsHint /* = 128 */) 
    {
        if (m_CurrentPool == VK_NULL_HANDLE) {
            m_CurrentPool = GrabPool(maxSetsHint);
            m_UsedPools.push_back(m_CurrentPool);
        }

        VkDescriptorSet set = VK_NULL_HANDLE;
        VkDescriptorSetAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
        allocInfo.descriptorPool = m_CurrentPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layout;

        VkResult res = vkAllocateDescriptorSets(m_Device, &allocInfo, &set);
        if (res == VK_SUCCESS) return set;

        if (res == VK_ERROR_FRAGMENTED_POOL || res == VK_ERROR_OUT_OF_POOL_MEMORY) {
            m_CurrentPool = GrabPool(std::max(2u * maxSetsHint, 64u));
            m_UsedPools.push_back(m_CurrentPool);

            allocInfo.descriptorPool = m_CurrentPool;
            VK_CHECK(vkAllocateDescriptorSets(m_Device, &allocInfo, &set));
            return set;
        }

        VK_CHECK(res);
        return VK_NULL_HANDLE;
    } 

    VkDescriptorPool DescriptorSetManager::GrabPool(u32 maxSets) 
    {

    }
}