#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "../../Core.h"
#include "../../Log.h"

namespace Lavendel {
	namespace RenderAPI {

		// Forward declaration to break circular dependency
		class GPUDevice;

		struct PipelineConfigInfo
		{
			VkViewport viewport;
			VkRect2D scissor;
			VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
			VkPipelineRasterizationStateCreateInfo rasterizationInfo;
			VkPipelineMultisampleStateCreateInfo multisampleInfo;
			VkPipelineColorBlendAttachmentState colorBlendAttachment;
			VkPipelineColorBlendStateCreateInfo colorBlendInfo;
			VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
			VkPipelineLayout pipelineLayout = nullptr;
			VkRenderPass renderPass = nullptr;
			uint32_t subpass = 0;
		};

		struct ProjectileInfo
		{
			float speed = 10.0f;
			float damage = 10.0f;
			float range = 100.0f;
			bool isExplosive = false;
			bool isLaser = false;
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

			void bind(VkCommandBuffer commandBuffer);

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