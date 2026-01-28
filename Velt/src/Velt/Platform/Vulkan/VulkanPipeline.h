#pragma once
#include "vtpch.h"
#include "Velt/Renderer/Pipeline.h"

namespace Velt::Renderer::RHI 
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
			VkPipelineColorBlendAttachmentState colorBlendAttachment;
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
			VulkanPipeline(const PipelineSpecification& specs); 
			~VulkanPipeline();

			VulkanPipeline(const VulkanPipeline&) = delete;
			void operator=(const VulkanPipeline&) = delete;

			virtual void Invalidate() override; 

			virtual void Bind(VkCommandBuffer& commandBuffer) override;
			virtual void BindDescriptorSet(VkCommandBuffer& commandBuffer, u32 frameIndex) override;
			virtual void UpdateDescriptorSet(u32 frameIndex, u32 binding, Ref<UniformBuffer> uniformBuffer) override;

			static void SetDefaultVulkanPipelineConfigInfo(VulkanPipelineConfigInfo& configInfo);

			void Init() override;

			virtual VkPipeline& GetVulkanPipeline() override { return m_VulkanPipeline; };
			virtual VkPipelineLayout& GetVulkanPipelineLayout() override { return m_ConfigInfo.pipelineLayout; };
			PipelineSpecification& GetSpecification() override { return m_Specification;  };
			const PipelineSpecification& GetSpecification() const override { return m_Specification; };

		private:
			static std::vector<char> ReadFile(const std::string& filepath);
			void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
			void CreatePipelineLayout();
			void CreateDescriptorResources();

			VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
			std::vector<VkDescriptorSet> m_DescriptorSets;
			VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
			
			VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
			PipelineSpecification m_Specification;
			BufferLayout m_Layout;
			VulkanPipelineConfigInfo m_ConfigInfo;
			VkPipeline m_VulkanPipeline;
			VkShaderModule m_VertexShaderModule;
			VkShaderModule m_FragmentShaderModule;

		};
}




