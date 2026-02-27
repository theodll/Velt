#pragma once
#include "Velt/Core/Core.h"
#include <string>
#include "Buffer.h"
#include "UniformBuffer.h"

namespace Velt
{
	class DescriptorLayoutCache;
	typedef VkDescriptorSetLayout DescriptorSetLayoutHandle;

	struct PipelineSpecification
	{
		BufferLayout Layout;	
		std::string VertexShaderPath; // this has to be the compiled binary right now, not the Source ! 
		std::string FragmentShaderPath; // this too
		std::string DebugName;
		std::vector<DescriptorSetLayoutHandle> SetLayouts;
	};

	class VELT_API Pipeline
	{
	public:
		virtual ~Pipeline() = default;

		virtual PipelineSpecification& GetSpecification() = 0;
		virtual const PipelineSpecification& GetSpecification() const = 0;

		virtual void Init() = 0;
		virtual void Invalidate() = 0;

		virtual void Bind(VkCommandBuffer& commandBuffer) = 0;
		virtual VkPipeline& GetVulkanPipeline() = 0;
		virtual VkPipelineLayout& GetVulkanPipelineLayout() = 0;
		virtual const std::vector<VkDescriptorSetLayout>& GetSetLayouts() const = 0;

		static Ref<Pipeline> Create(const PipelineSpecification& spec);
	};

}