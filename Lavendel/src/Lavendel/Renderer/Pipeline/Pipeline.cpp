#include "lvpch.h"

#include "Pipeline.h"
#include "../Core/Device.h"
#include "../Model/Model.h"





namespace Lavendel {
	namespace RenderAPI {

		Pipeline::~Pipeline()
		{
			LV_PROFILE_FUNCTION();
			
			vkDestroyShaderModule(m_Device.device(), fragShaderModule, nullptr);
			vkDestroyShaderModule(m_Device.device(), vertShaderModule, nullptr);
			vkDestroyPipeline(m_Device.device(), graphicsPipeline, nullptr);
		}

		Pipeline::Pipeline(GPUDevice& device, const std::string& vertFilepath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo) 
			: m_Device{ device }
		{
			LV_PROFILE_FUNCTION();
			LV_CORE_INFO("Creating Pipeline...");
			createGraphicsPipeline(vertFilepath, fragFilePath, configInfo);
		}

		void Pipeline::bind(VkCommandBuffer commandBuffer)
		{
			LV_PROFILE_FUNCTION();
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		}

		std::vector<char> Pipeline::readFile(const std::string& filepath)
		{
			LV_PROFILE_FUNCTION();
			auto file = std::ifstream(filepath, std::ios::ate | std::ios::binary);

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
			LV_PROFILE_FUNCTION();
			auto vertCode = readFile(vertShaderPath);
			auto fragCode = readFile(fragShaderPath);
			
			createShaderModule(vertCode, &vertShaderModule);
			createShaderModule(fragCode, &fragShaderModule);

			VkPipelineShaderStageCreateInfo shaderStages[2];

			// vert shader stage
			shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
			shaderStages[0].module = vertShaderModule;
			shaderStages[0].pName = "main";
			shaderStages[0].flags = 0;
			shaderStages[0].pNext = nullptr;
			shaderStages[0].pSpecializationInfo = nullptr;

			// frag shader stage
			shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			shaderStages[1].module = fragShaderModule;
			shaderStages[1].pName = "main";
			shaderStages[1].flags = 0;
			shaderStages[1].pNext = nullptr;
			shaderStages[1].pSpecializationInfo = nullptr;

			auto bindingDescriptions = Model::Vertex::getBindingDescription();
			auto attributeDescriptions = Model::Vertex::getAttributeDescription();
			VkPipelineVertexInputStateCreateInfo vertexInputInfo{};

			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
			vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
			vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
			
			VkPipelineViewportStateCreateInfo viewportInfo{};

			VkGraphicsPipelineCreateInfo pipelineInfo{};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			// Pass through configInfo data to corresponding members
			pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
			pipelineInfo.pViewportState = &configInfo.viewportInfo;
			pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
			pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
			pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
			pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
			pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;	// Optional

			pipelineInfo.layout = configInfo.pipelineLayout;
			pipelineInfo.renderPass = configInfo.renderPass;
			pipelineInfo.subpass = configInfo.subpass;

			pipelineInfo.basePipelineIndex = -1; // Optional
			pipelineInfo.basePipelineHandle = nullptr; // Optional

			if (vkCreateGraphicsPipelines(m_Device.device(), nullptr, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) 
			{
				LV_CORE_ERROR("Failed to create graphics pipeline!");
				throw std::runtime_error("failed to create graphics pipeline!");
			}
		}

		void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) 
		{
			LV_PROFILE_FUNCTION();
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

		void Pipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
		{
			LV_PROFILE_FUNCTION();
			// Input Assembly

			configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

			// Viewport 

			configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO; // set the struct type member
			configInfo.viewportInfo.viewportCount = 1;
			configInfo.viewportInfo.scissorCount = 1;
			configInfo.viewportInfo.pViewports = nullptr;
			configInfo.viewportInfo.pScissors = nullptr;

			// Rasterization Info
			configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO; // set the struct type member
			configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;	// forces z component to be between 0 and 1 (we dont want that!). When using this you have to enable a GPU Feature.
			configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;  // discards all primitives before rasterization stage, by default rasterization is needed, so set to false
			configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;  // how to handle polygons, by default they should fill in.
			configInfo.rasterizationInfo.lineWidth = 1.0f;
			configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;	// Cull Triangles by facing direction, for debug stages it is better to disable culling.
			configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; // Which vertex order is considered front facing. In vulkan the origin is top left so we use clockwise.
			configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;	// alter depth values, useful for decals and shadow maps.
			configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
			configInfo.rasterizationInfo.depthBiasClamp = 0.0f;	// Optional
			configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;  // Optional


			// Multisampling Info
			configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;	// set the struct type member
			configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;	// disable sample shading, enable for better quality
			configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			configInfo.multisampleInfo.minSampleShading = 1.0f;
			configInfo.multisampleInfo.pSampleMask = nullptr;
			configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
			configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

			configInfo.colorBlendAttachment.colorWriteMask = 
				VK_COLOR_COMPONENT_R_BIT | 
				VK_COLOR_COMPONENT_G_BIT | 
				VK_COLOR_COMPONENT_B_BIT | 
				VK_COLOR_COMPONENT_A_BIT;
			configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
			configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;	// Optional
			configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;				// Optional
			configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;	// Optional
			configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
			configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;				// Optional

			// Color Blending Info
			configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO; // set the struct type member
			configInfo.colorBlendInfo.logicOpEnable = VK_FALSE; // disable logical operations
			configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
			configInfo.colorBlendInfo.attachmentCount = 1;
			configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
			configInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
			configInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
			configInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
			configInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

			// Depth Stencil Info
			configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO; // set the struct type member
			configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
			configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
			configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
			configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
			configInfo.depthStencilInfo.minDepthBounds = 0.0f; // Optional
			configInfo.depthStencilInfo.maxDepthBounds = 1.0f; // Optional
			configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
			configInfo.depthStencilInfo.front = {}; // Optional
			configInfo.depthStencilInfo.back = {};  // Optional

			configInfo.dynamicStatesEnable = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};
			configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO; // set the struct type member
			configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStatesEnable.data();
			configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStatesEnable.size());
			configInfo.dynamicStateInfo.flags = 0;

		}

	}
}