#include "Pipeline.h"
#include "Device.h"
#include <fstream>
#include <vector>
#include <string>

namespace Lavendel {
	namespace RendererAPI {

		Pipeline::Pipeline(GPUDevice& device, const std::string& vertFilepath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo) 
			: m_Device{ device }
		{
			createGraphicsPipeline(vertFilepath, fragFilePath, configInfo);
		}

		Pipeline::~Pipeline()
		{
			// Cleanup code here if needed
		}

		std::vector<char> Pipeline::readFile(const std::string& filepath)
		{
			std::ifstream file(filepath, std::ios::ate | std::ios::binary);

			if (!file)
			{
				LV_CORE_ERROR("Failed to open file: {}", filepath);
				throw std::runtime_error("Failed to open file: " + filepath);  
			}

			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.close();

			return buffer;
		}

		void Pipeline::createGraphicsPipeline(const std::string& vertShaderPath, const std::string& fragShaderPath, const PipelineConfigInfo& configInfo)
		{
			auto vertCode = readFile(vertShaderPath);
			auto fragCode = readFile(fragShaderPath);

			LV_CORE_INFO("Vertex Shader Code Size: {}", vertCode.size());
			LV_CORE_INFO("Fragment Shader Code Size: {}", fragCode.size());
		}

		void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) 
		{
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

			if (vkCreateShaderModule(m_Device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) 
			{
				LV_CORE_ERROR("Failed to create shader module!");
				throw std::runtime_error("failed to create shader module!");
			}
		}

		PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
		{
			PipelineConfigInfo configInfo{};

			configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

			configInfo.viewport.x = 0.0f;
			configInfo.viewport.y = 0.0f;
			configInfo.viewport.width = static_cast<float>(width);
			configInfo.viewport.height = static_cast<float>(height);
			configInfo.viewport.minDepth = 0.0f;
			configInfo.viewport.maxDepth = 1.0f;

			configInfo.scissor.offset = { 0, 0 };
			configInfo.scissor.extent = { width, height };

			return configInfo;
		}

	}
}