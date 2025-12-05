#pragma once
#include "vtpch.h"

namespace Velt {
	namespace RenderAPI {

		// Forward declaration to break circular dependency
		class Velt_API GPUDevice;

		struct PipelineConfigInfo
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

		class Velt_API Pipeline
		{
		public:
			Pipeline(GPUDevice& device,
					 const std::string& vertFilepath,
					 const std::string& fragFilePath,
					 const PipelineConfigInfo& configInfo);

			~Pipeline();

			Pipeline(const Pipeline&) = delete;
			void operator=(const Pipeline&) = delete;

			void bind(VkCommandBuffer commandBuffer);

			static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

		private:
			static std::vector<char> readFile(const std::string& filepath);
			void createGraphicsPipeline(const std::string& vertShaderPath,
										const std::string& fragShaderPath,
										const PipelineConfigInfo& configInfo);

			void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

			// Member variable for GPUDevice reference
			GPUDevice& m_Device;

			VkPipeline graphicsPipeline;
			VkShaderModule vertShaderModule;
			VkShaderModule fragShaderModule;
		};
	}
}