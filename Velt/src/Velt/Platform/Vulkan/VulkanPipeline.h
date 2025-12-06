#pragma once
#include "vtpch.h"

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
		uint32_t subpass = 0;
	};

	class VELT_API VulkanPipeline
	{
	public:
		VulkanPipeline(VulkanDevice &device,
					   const std::string &vertFilepath,
					   const std::string &fragFilePath,
					   const VulkanPipelineConfigInfo &configInfo);

		~VulkanPipeline();

		VulkanPipeline(const VulkanPipeline &) = delete;
		void operator=(const VulkanPipeline &) = delete;

		void bind(VkCommandBuffer commandBuffer);

		static void defaultVulkanPipelineConfigInfo(VulkanPipelineConfigInfo &configInfo);

	private:
		static std::vector<char> readFile(const std::string &filepath);
		void createGraphicsVulkanPipeline(const std::string &vertShaderPath,
										  const std::string &fragShaderPath,
										  const VulkanPipelineConfigInfo &configInfo);

		void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

		// Member variable for VulkanDevice reference
		VulkanDevice &m_Device;

		VkPipeline graphicsVulkanPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
	};
}