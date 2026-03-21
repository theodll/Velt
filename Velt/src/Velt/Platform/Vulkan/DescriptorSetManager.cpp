#include "vtpch.h"
#include "Core/Core.h"
#include "DescriptorSetManager.h"

#include "../../../../vendor/sdl3/src/video/khronos/vulkan/vulkan_core.h"
#include "../../Core/Assert.h"

namespace Velt::RHI 
{
    void DescriptorSetManager::Init() {} 

    void DescriptorSetManager::Shutdown() {}

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

    void DescriptorSetManager::WriteImage(VkDescriptorSet set, u32 binding, const VkDescriptorImageInfo& info)
    {
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = set;
        write.dstBinding = binding;
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        write.pImageInfo = &info;

        vkUpdateDescriptorSets(m_Device.device(), 1, &write, 0, nullptr);
    }

    void DescriptorSetManager::WriteSampler(VkDescriptorSet set, u32 binding, VkSampler sampler)
    {
        VkDescriptorImageInfo info{};
        info.sampler = sampler;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = set;
        write.dstBinding = binding;
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        write.pImageInfo = &info;

        vkUpdateDescriptorSets(m_Device.device(), 1, &write, 0, nullptr);
    }

    void DescriptorSetManager::WriteBuffer(VkDescriptorSet set, uint32_t binding, VkBuffer buffer, VkDeviceSize size)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = size;

        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = set;
        write.dstBinding = binding;
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(m_Device.device(), 1, &write, 0, nullptr);
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

        VkResult res = vkAllocateDescriptorSets(m_Device.device(), &allocInfo, &set);
        if (res == VK_SUCCESS) return set;

        if (res == VK_ERROR_FRAGMENTED_POOL || res == VK_ERROR_OUT_OF_POOL_MEMORY) {
            m_CurrentPool = GrabPool(std::max(2u * maxSetsHint, 64u));
            m_UsedPools.push_back(m_CurrentPool);

            allocInfo.descriptorPool = m_CurrentPool;
            if (vkAllocateDescriptorSets(m_Device.device(), &allocInfo, &set) != VK_SUCCESS)
                VT_CORE_ASSERT(false, "Failed to allocate Descriptor Sets")
            return set;
        }

        if (res != VK_SUCCESS)
            VT_CORE_ASSERT("Failed to allocate Descripor Sets {}", VkResultToString(res)); 

        return VK_NULL_HANDLE;
    }

    VkDescriptorPool DescriptorSetManager::CreatePool(u32 maxSets)
    {
        std::vector<VkDescriptorPoolSize> poolSizes;
        poolSizes.reserve(m_PoolSizes.sizes.size());

        for (auto [type, weight] : m_PoolSizes.sizes )
        {
            VkDescriptorPoolSize s{};
            s.type = type;
            s.descriptorCount = (u32)std::ceil(weight * maxSets);
        	poolSizes.push_back(s);
        }

        VkDescriptorPoolCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        info.flags = 0; 
        info.maxSets = maxSets;
        info.poolSizeCount = (u32)poolSizes.size();
        info.pPoolSizes = poolSizes.data();

        VkDescriptorPool pool = VK_NULL_HANDLE;
        if (vkCreateDescriptorPool(m_Device.device(), &info, nullptr, &pool) != VK_SUCCESS)
        {
	        VT_CORE_ASSERT(false, "Failed to create Descriptor Pools")
        }
        return pool;
    }

    VkDescriptorPool DescriptorSetManager::GrabPool(u32 maxSets) 
    {
        if (!m_FreePools.empty()) {
            VkDescriptorPool pool = m_FreePools.back();
            m_FreePools.pop_back();
            return pool;
        }
        return CreatePool(maxSets);
    }
}
