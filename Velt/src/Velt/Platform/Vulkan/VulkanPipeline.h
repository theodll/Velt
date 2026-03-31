#pragma once
#include "vtpch.h"
#include "Velt/Renderer/Pipeline.h"

namespace Velt::RHI
{
	class VELT_API VulkanDevice;

	struct VulkanPipelineConfigInfo
	{
		std::vector<VkFormat> colorAttachmentFormats;
		VkFormat depthAttachmentFormat;
		VkFormat stencilAttachmentFormat;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;

		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;

		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStatesEnable;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		u32 subpass = 0;
	};

	class VELT_API VulkanPipeline : public Pipeline
	{
	public:
		VulkanPipeline(const PipelineSpecification* pSpecs);
		~VulkanPipeline();

		VulkanPipeline(const VulkanPipeline&) = delete;
		void operator=(const VulkanPipeline&) = delete;

		void Init() override;
		void Shutdown() override;
		virtual void Invalidate() override;

		virtual void Bind(VkCommandBuffer& commandBuffer) override;
		static void SetDefaultVulkanPipelineConfigInfo(VulkanPipelineConfigInfo* pConfigInfo);

		virtual VkPipeline& GetVulkanPipeline() override { return m_VulkanPipeline; };
		virtual VkPipelineLayout& GetVulkanPipelineLayout() override { return m_PipelineLayout; };
		PipelineSpecification& GetSpecification() override { return m_Specification; };
		const PipelineSpecification& GetSpecification() const override { return m_Specification; };
		virtual const std::vector<VkDescriptorSetLayout>& GetSetLayouts() const override { return m_Specification.SetLayouts; }

	private:
		void CreatePipelineLayout();

		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		static PipelineSpecification m_Specification;
		VertexLayout m_Layout;
		VulkanPipelineConfigInfo m_ConfigInfo;
		VkPipeline m_VulkanPipeline;
	};
}