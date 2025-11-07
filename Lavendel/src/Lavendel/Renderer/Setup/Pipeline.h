#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "../../Core.h"
#include "../../Log.h"

namespace Lavendel {
	namespace RendererAPI {

		// Forward declaration to break circular dependency
		class GPUDevice;

		struct PipelineConfigInfo
		{
			VkViewport viewport;
			VkRect2D scissor;
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
			VkPipelineViewportStateCreateInfo viewportInfo;
			VkPipelineRasterizationStateCreateInfo rasterizationInfo;
			VkPipelineMultisampleStateCreateFlags multisampleInfo;
			VkPipelineColorBlendAttachmentState colorBlendAttachment;
			VkPipelineColorBlendStateCreateInfo colorBlendInfo;
			VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
			VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
			VkRenderPass renderPass = VK_NULL_HANDLE;
			uint32_t subpass = 0;

		};
		class LAVENDEL_API Pipeline
		{
		public:
			Pipeline(GPUDevice& device,
					 const std::string& vertFilepath,
					 const std::string& fragFilePath,
					 const PipelineConfigInfo& configInfo);

			~Pipeline();

			Pipeline(const Pipeline&) = delete;
			void operator=(const Pipeline&) = delete;

			static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

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