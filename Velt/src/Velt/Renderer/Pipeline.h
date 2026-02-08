#pragma once
#include "Velt/Core/Core.h"
#include <string>
#include "Buffer.h"
#include "UniformBuffer.h"

namespace Velt
{
	struct PipelineSpecification
	{
		BufferLayout Layout;	
		std::string VertexShaderPath; // this has to be the compiled binary right now, not the Source ! 
		std::string FragmentShaderPath; // this too
		std::string DebugName;
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
		virtual void BindDescriptorSet(VkCommandBuffer& commandBuffer, u32 frameIndex) = 0;
		virtual void UpdateDescriptorSet(u32 frameIndex, u32 binding, Ref<UniformBuffer> uniformBuffer) = 0;
		virtual VkPipeline& GetVulkanPipeline() = 0;
		virtual VkPipelineLayout& GetVulkanPipelineLayout() = 0;

		static Ref<Pipeline> Create(const PipelineSpecification& spec);
	};

}