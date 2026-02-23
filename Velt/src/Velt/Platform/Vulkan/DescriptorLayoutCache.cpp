#include "vtpch.h"
#include "Core/Core.h"
#include "DescriptorLayoutCache.h"

namespace Velt::RHI
{
	static VkDescriptorType ToVulkanDescriptorType(DescriptorType type)
	{
		switch (type)
		{
		case DescriptorType::SAMPLER:
			return VK_DESCRIPTOR_TYPE_SAMPLER;

		case DescriptorType::SAMPLED_IMAGE:
			return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

		case DescriptorType::COMBINED_IMAGE_SAMPLER:
			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		case DescriptorType::STORAGE_BUFFER:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

		case DescriptorType::STORAGE_BUFFER_DYNAMIC:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;

		case DescriptorType::STORAGE_TEXEL_BUFFER:
			return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;

		case DescriptorType::STORAGE_IMAGE:
			return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;

		case DescriptorType::INPUT_ATTACHMENT:
			return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

		case DescriptorType::UNIFORM_BUFFER: 
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; 
			
		case DescriptorType::UNIFORM_BUFFER_DYNAMIC:
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;

		case DescriptorType::UNIFORM_TEXEL_BUFFER:
			return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;

		default:
			VT_CORE_ASSERT(false, "");
			return VK_DESCRIPTOR_TYPE_MAX_ENUM;
		}
	}
	static VkShaderStageFlagBits ToVulkanShaderStage(ShaderStage stage)
	{
		switch (stage)
		{
		case ShaderStage::FRAGMENT:
			VK_SHADER_STAGE_FRAGMENT_BIT;

		case ShaderStage::VERTEX:
			VK_SHADER_STAGE_VERTEX_BIT;

		default:
			VT_CORE_ASSERT(false, "");
			return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		}
	}

	void DescriptorLayoutCache::Init()
	{
	}

	void DescriptorLayoutCache::Shutdown()
	{
	}

	DescriptorSetLayoutHandle DescriptorLayoutCache::CreateLayout(std::vector<DescriptorBinding> *bindings)
	{
		auto hash = HashBindings(bindings);
		auto it = m_Cache.find(hash);

		if (it != m_Cache.end())
			return it->second;

		VkDescriptorSetLayout vkLayout = CreateVulkanLayout(bindings);
		m_Cache[hash] = vkLayout;

		return vkLayout;
	}

	size_t DescriptorLayoutCache::HashBindings(std::vector<DescriptorBinding>* bindings)
	{
		size_t hash{};

		for (const auto& b : *bindings)
		{
			HashCombine(&hash, b.binding);
			HashCombine(&hash, (u32)b.type);
			HashCombine(&hash, b.count);
			HashCombine(&hash, (u32)b.stage);
		}

		return hash;
	}

	VkDescriptorSetLayout DescriptorLayoutCache::CreateVulkanLayout(std::vector<DescriptorBinding> *pBindings)
	{
		std::vector<VkDescriptorSetLayoutBinding> vkBindings;

		for (auto& b : *pBindings)
		{
			VkDescriptorSetLayoutBinding vkB{};
			vkB.binding = b.binding;
			vkB.descriptorCount = b.count;
			vkB.descriptorType = ToVulkanDescriptorType(b.type);
			vkB.stageFlags = ToVulkanShaderStage(b.stage);
			vkB.pImmutableSamplers = VT_NULL_HANDLE;
			vkBindings.push_back(vkB);
		}

		VkDescriptorSetLayoutCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		info.bindingCount = (u32)vkBindings.size();
		info.pBindings = vkBindings.data();

		VkDescriptorSetLayout layout;
		VT_VK_CHECK(vkCreateDescriptorSetLayout(VulkanContext::GetDevice().device(), &info, VT_NULL_HANDLE, &layout), "Failed to create Descriptor Set Layouts");
		return layout;
	}
}
