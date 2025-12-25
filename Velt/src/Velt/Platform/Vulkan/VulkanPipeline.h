#pragma once
#include "vtpch.h"
#include "Velt/Renderer/Pipeline.h"

namespace Velt::Renderer::Vulkan 
{

		class VELT_API VulkanDevice;

		struct VulkanPipelineConfigInfo
		{
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

		class VELT_API VulkanPipeline : public Renderer::Pipeline
		{
		public:
			VulkanPipeline(PipelineSpecification& specs); 
			~VulkanPipeline();

			VulkanPipeline(const VulkanPipeline&) = delete;
			void operator=(const VulkanPipeline&) = delete;

			virtual void Invalidate() override; 

			void Bind(VkCommandBuffer commandBuffer);

			static void defaultVulkanPipelineConfigInfo(VulkanPipelineConfigInfo& configInfo);

		private:
			static std::vector<char> readFile(const std::string& filepath);
			void createGraphicsVulkanPipeline(const std::string& vertShaderPath,
										const std::string& fragShaderPath,
										const VulkanPipelineConfigInfo& configInfo);

			void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

			PipelineSpecification m_Specification;
			BufferLayout m_Layout;
			VulkanPipelineConfigInfo m_ConfigInfo;
			VkPipeline m_VulkanPipeline;
			VkShaderModule m_VertexShaderModule;
			VkShaderModule m_FragmentShaderModule;
		};
}





//namespace Velt::Renderer::Vulkan
//{
//
//	class VELT_API VulkanDevice;
//
//	struct VulkanPipelineConfigInfo
//	{
//		VkPipelineViewportStateCreateInfo viewportInfo;
//		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
//		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
//		VkPipelineMultisampleStateCreateInfo multisampleInfo;
//		VkPipelineColorBlendAttachmentState colorBlendAttachment;
//		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
//		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
//		std::vector<VkDynamicState> dynamicStatesEnable;
//		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
//		VkPipelineLayout pipelineLayout = nullptr;
//		VkRenderPass renderPass = nullptr;
//		u32 subpass = 0;
//	};
//
//	class VELT_API VulkanPipeline : public Renderer::Pipeline
//	{
//	public:
//		VulkanPipeline(PipelineSpecification& specs);
//		~VulkanPipeline();
//
//		VulkanPipeline(const VulkanPipeline&) = delete;
//		void operator=(const VulkanPipeline&) = delete;
//
//		virtual void Invalidate() override;
//
//		void bind(VkCommandBuffer commandBuffer);
//
//		static void defaultVulkanPipelineConfigInfo(VulkanPipelineConfigInfo& configInfo);
//
//	private:
//		static std::vector<char> readFile(const std::string& filepath);
//		void createGraphicsVulkanPipeline(const std::string& vertShaderPath,
//			const std::string& fragShaderPath,
//			const VulkanPipelineConfigInfo& configInfo);
//
//		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
//
//		// Member variable for VulkanDevice reference
//		VulkanDevice& m_Device;
//
//		VkPipeline graphicsVulkanPipeline;
//		VkShaderModule vertShaderModule;
//		VkShaderModule fragShaderModule;
//	};
//}