#include "vtpch.h"
#include "Core/Core.h"
#include "VulkanPipeline.h"
#include "VulkanContext.h"

namespace Velt::Renderer::Vulkan {

	VulkanPipeline::~VulkanPipeline()
	{
		VkDevice device = VulkanContext::GetDevice().device();
		vkDestroyPipeline(device, m_VulkanPipeline, nullptr);
	}


	VulkanPipeline::VulkanPipeline(const PipelineSpecification& specs) : m_Specification(specs), m_Layout(specs.Layout)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Create Pipeline");
		SetDefaultVulkanPipelineConfigInfo(m_ConfigInfo);
	}


	static VkFormat ShaderDataTypeToVulkanFormat(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:  return VK_FORMAT_R32_SFLOAT;
		case ShaderDataType::Float2: return VK_FORMAT_R32G32_SFLOAT;
		case ShaderDataType::Float3: return VK_FORMAT_R32G32B32_SFLOAT;
		case ShaderDataType::Float4: return VK_FORMAT_R32G32B32A32_SFLOAT;
		case ShaderDataType::Int:    return VK_FORMAT_R32_SINT;
		case ShaderDataType::Int2:   return VK_FORMAT_R32G32_SINT;
		case ShaderDataType::Int3:   return VK_FORMAT_R32G32B32_SINT;
		case ShaderDataType::Int4:   return VK_FORMAT_R32G32B32A32_SINT;
		case ShaderDataType::Bool:   return VK_FORMAT_R8_UINT;
		default:
			VT_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return VK_FORMAT_UNDEFINED;
		}
		return VK_FORMAT_UNDEFINED;
	}

	void VulkanPipeline::Invalidate()
	{
		auto vertCode = ReadFile(m_Specification.VertexShaderPath);
		auto fragCode = ReadFile(m_Specification.FragmentShaderPath);

		VkShaderModule vertModule, fragModule;
		CreateShaderModule(vertCode, &vertModule);
		CreateShaderModule(fragCode, &fragModule);

		SetDefaultVulkanPipelineConfigInfo(m_ConfigInfo);

		VkPipelineShaderStageCreateInfo shaderStages[2] = {};
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertModule;
		shaderStages[0].pName = "main";

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragModule;
		shaderStages[1].pName = "main";

		VkVertexInputBindingDescription bindingDesc{};
		bindingDesc.binding = 0;
		bindingDesc.stride = m_Layout.getStride();
		bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::vector<VkVertexInputAttributeDescription> attributeDescs;
		uint32_t location = 0;
		for (const auto& element : m_Layout.GetElements())
		{
			if (element.Type == ShaderDataType::Mat3 || element.Type == ShaderDataType::Mat4)
			{
				uint32_t count = (element.Type == ShaderDataType::Mat4) ? 4 : 3;
				for (uint32_t i = 0; i < count; i++)
				{
					auto& attr = attributeDescs.emplace_back();
					attr.binding = 0;
					attr.location = location++;
					attr.format = VK_FORMAT_R32G32B32A32_SFLOAT; 
					attr.offset = element.Offset + (i * (count * 4));
				}
			}
			else
			{
				auto& attr = attributeDescs.emplace_back();
				attr.binding = 0;
				attr.location = location++;
				attr.format = ShaderDataTypeToVulkanFormat(element.Type);
				attr.offset = element.Offset;
			}
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.pNext = nullptr;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDesc;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescs.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescs.data();

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = nullptr;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;

		pipelineInfo.pInputAssemblyState = &m_ConfigInfo.inputAssemblyInfo;
		pipelineInfo.pViewportState = &m_ConfigInfo.viewportInfo;
		pipelineInfo.pRasterizationState = &m_ConfigInfo.rasterizationInfo;
		pipelineInfo.pMultisampleState = &m_ConfigInfo.multisampleInfo;
		pipelineInfo.pDepthStencilState = &m_ConfigInfo.depthStencilInfo;
		pipelineInfo.pColorBlendState = &m_ConfigInfo.colorBlendInfo;
		pipelineInfo.pDynamicState = &m_ConfigInfo.dynamicStateInfo;
		pipelineInfo.layout = m_ConfigInfo.pipelineLayout;
		pipelineInfo.renderPass = m_ConfigInfo.renderPass;
		pipelineInfo.subpass = m_ConfigInfo.subpass;

		if (vkCreateGraphicsPipelines(VulkanContext::GetDevice().device(), nullptr, 1, &pipelineInfo, nullptr, &m_VulkanPipeline) != VK_SUCCESS)
		{
			VT_CORE_ASSERT(false, "Failed to create Graphics Pipeline")
		}

		vkDestroyShaderModule(VulkanContext::GetDevice().device(), vertModule, nullptr);
		vkDestroyShaderModule(VulkanContext::GetDevice().device(), fragModule, nullptr);
	}

	void VulkanPipeline::Bind(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VulkanPipeline);
	}

	void VulkanPipeline::SetDefaultVulkanPipelineConfigInfo(VulkanPipelineConfigInfo& configInfo)
	{
		VT_PROFILE_FUNCTION();

		// Input Assembly
		configInfo.inputAssemblyInfo = {};  // ✅ Zero-initialize first! 
		configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		// Viewport 
		configInfo.viewportInfo = {};  // ✅ Zero-initialize first!
		configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.viewportInfo.viewportCount = 1;
		configInfo.viewportInfo.scissorCount = 1;
		configInfo.viewportInfo.pViewports = nullptr;
		configInfo.viewportInfo.pScissors = nullptr;

		// Rasterization Info
		configInfo.rasterizationInfo = {};  // ✅ Zero-initialize first! 
		configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.rasterizationInfo.lineWidth = 1.0f;
		configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;
		configInfo.rasterizationInfo.depthBiasClamp = 0.0f;
		configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;

		// Multisampling Info
		configInfo.multisampleInfo = {};  // ✅ Zero-initialize first!
		configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.multisampleInfo.minSampleShading = 1.0f;
		configInfo.multisampleInfo.pSampleMask = nullptr;
		configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;

		// Color Blend Attachment
		configInfo.colorBlendAttachment = {};  // ✅ Zero-initialize first! 
		configInfo.colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		configInfo.colorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		configInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		configInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		configInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		// Color Blending Info
		configInfo.colorBlendInfo = {};  // ✅ Zero-initialize first! 
		configInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		configInfo.colorBlendInfo.attachmentCount = 1;
		configInfo.colorBlendInfo.pAttachments = &configInfo.colorBlendAttachment;
		configInfo.colorBlendInfo.blendConstants[0] = 0.0f;
		configInfo.colorBlendInfo.blendConstants[1] = 0.0f;
		configInfo.colorBlendInfo.blendConstants[2] = 0.0f;
		configInfo.colorBlendInfo.blendConstants[3] = 0.0f;

		// Depth Stencil Info
		configInfo.depthStencilInfo = {};  // ✅ Zero-initialize first!
		configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.minDepthBounds = 0.0f;
		configInfo.depthStencilInfo.maxDepthBounds = 1.0f;
		configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.depthStencilInfo.front = {};
		configInfo.depthStencilInfo.back = {};

		// Dynamic State
		configInfo.dynamicStatesEnable = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
		configInfo.dynamicStateInfo = {};  // ✅ Zero-initialize first! 
		configInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo.dynamicStateInfo.pDynamicStates = configInfo.dynamicStatesEnable.data();
		configInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStatesEnable.size());
		configInfo.dynamicStateInfo.flags = 0;
	}

	std::vector<char> VulkanPipeline::ReadFile(const std::string& filepath)
	{
		auto file = std::ifstream(filepath, std::ios::ate | std::ios::binary);

		VT_CORE_ASSERT(file, "Failed to open file: {}", filepath);

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	void VulkanPipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(VulkanContext::GetDevice().device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
		{
			VT_CORE_ERROR("Failed to create shader module!");
		}
	}

	
}