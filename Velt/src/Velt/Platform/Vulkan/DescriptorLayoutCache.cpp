#include "vtpch.h"
#include "Core/Core.h"
#include "DescriptorLayoutCache.h"

namespace Velt::RHI
{
	void DescriptorLayoutCache::Init()
	{
	}

	void DescriptorLayoutCache::Shutdown()
	{
	}

	DSLayoutHandle DescriptorLayoutCache::CreateLayout(const std::vector<DescriptorBinding>& bindings)
	{
		
	}

	VkDescriptorSetLayout DescriptorLayoutCache::CreateVulkanLayout(const std::vector<DescriptorBinding>& bindings)
	{
	}
}
