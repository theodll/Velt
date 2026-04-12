#pragma once
#include "Velt/Core/Core.h"
#include <string>
#include "Buffer.h"
#include "ShaderLibrary.h"
#include "UniformBuffer.h"

namespace Velt
{
	class DescriptorLayoutCache;
	typedef VkDescriptorSetLayout DescriptorSetLayoutHandle;

	enum CullMode {
		VT_CULL_MODE_NONE = 0,
		VT_CULL_MODE_FRONT_BIT = 0x00000001,
		VT_CULL_MODE_BACK_BIT = 0x00000002,
		VT_CULL_MODE_FRONT_AND_BACK = 0x00000003
	};

	struct PipelineSpecification
	{
		VertexLayout Layout;	
		Ref<Shader> VertexShader;
		Ref<Shader> FragmentShader;
		std::vector<DescriptorSetLayoutHandle> SetLayouts;
		std::vector<VkFormat> ColorAttachmentFormats;
		Velt::CullMode CullMode;
	};

	// Todo [10.03.26, Theo]: move somewhere else

	struct BlendState
	{
		bool enable;

		VkBlendFactor srcColor;
		VkBlendFactor dstColor;
		VkBlendOp colorOp;

		VkBlendFactor srcAlpha;
		VkBlendFactor dstAlpha;
		VkBlendOp alphaOp;

		VkColorComponentFlags writeMask;
	};

	struct DepthState
	{
		bool depthTest;
		bool depthWrite;
		VkCompareOp compare;
	};

	class VELT_API Pipeline
	{
	public:
		virtual ~Pipeline() = default;

		virtual PipelineSpecification& GetSpecification() = 0;
		virtual const PipelineSpecification& GetSpecification() const = 0;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void Invalidate() = 0;

		virtual void Bind(VkCommandBuffer& commandBuffer) = 0;
		virtual VkPipeline& GetVulkanPipeline() = 0;
		virtual VkPipelineLayout& GetVulkanPipelineLayout() = 0;
		virtual const std::vector<VkDescriptorSetLayout>& GetSetLayouts() const = 0;

		static Ref<Pipeline> Create(const PipelineSpecification* pSpec);
	};

}