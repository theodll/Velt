#include "vtpch.h"
#include "Core/Core.h"
#include "VulkanPipeline.h"
#include "VulkanContext.h"
#include "Core/Application.h"

namespace Velt::RHI {

	VulkanPipeline::~VulkanPipeline()
	{
		auto& device = VulkanContext::GetDevice()->device();
		
		vkDestroyPipeline(device, m_VulkanPipeline, nullptr);
		vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
	}

	VulkanPipeline::VulkanPipeline(const PipelineSpecification* pSpecs) : m_Specification(*pSpecs), m_Layout(pSpecs->Layout)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Create Pipeline");
		VT_CORE_ERROR("{}", m_Specification.Layout.GetStride());
	}

	void VulkanPipeline::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Init Pipeline");
		SetDefaultVulkanPipelineConfigInfo(&m_ConfigInfo);
		CreatePipelineLayout();
		Invalidate();
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

	void VulkanPipeline::CreatePipelineLayout()
	{
		VT_PROFILE_FUNCTION();

		VkPushConstantRange range = {};
		range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT /* | VK_SHADER_STAGE_FRAGMENT_BIT */;
		range.offset = 0;
		range.size = sizeof(glm::mat4);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = nullptr;
		pipelineLayoutInfo.flags = 0;
		pipelineLayoutInfo.setLayoutCount = (u32)m_Specification.SetLayouts.size();
		pipelineLayoutInfo.pSetLayouts = m_Specification.SetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &range;
		if (vkCreatePipelineLayout(VulkanContext::GetDevice()->device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			VT_CORE_ERROR("Failed to create pipeline layout!");
		};

		m_ConfigInfo.pipelineLayout = m_PipelineLayout;
	}

	void VulkanPipeline::Invalidate()
	{
		auto vertCode = ReadFile(m_Specification.VertexShaderPath);
		auto fragCode = ReadFile(m_Specification.FragmentShaderPath);

		VkShaderModule vertModule, fragModule;
		CreateShaderModule(vertCode, &vertModule);
		CreateShaderModule(fragCode, &fragModule);

		VkPipelineShaderStageCreateInfo shaderStages[2] = {};
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = vertModule;
		shaderStages[0].pName = "main";
		shaderStages[0].pNext = nullptr;
		shaderStages[0].flags = 0;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = fragModule;
		shaderStages[1].pName = "main";
		shaderStages[1].pNext = nullptr;
		shaderStages[1].flags = 0;
		shaderStages[1].pSpecializationInfo = nullptr;

		VkVertexInputBindingDescription bindingDesc{};
		bindingDesc.binding = 0;
		bindingDesc.stride = m_Layout.GetStride();
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
					VkVertexInputAttributeDescription attr{};
					attr.binding = 0;
					attr.location = location++;
					attr.format = VK_FORMAT_R32G32B32A32_SFLOAT;
					attr.offset = 0 + (i * (count * 4));
					attributeDescs.push_back(attr);
				}
			}
			else
			{
				VkVertexInputAttributeDescription attr{};
				attr.binding = 0;
				attr.location = location++;
				attr.format = ShaderDataTypeToVulkanFormat(element.Type);
				attr.offset = 0;
				attributeDescs.push_back(attr);
			}
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.pNext = nullptr;
		vertexInputInfo.flags = 0;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDesc;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescs.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescs.data();

		VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo{};
		pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		pipelineRenderingCreateInfo.pNext = nullptr;
		pipelineRenderingCreateInfo.colorAttachmentCount = static_cast<uint32_t>(m_ConfigInfo.colorAttachmentFormats.size());
		pipelineRenderingCreateInfo.pColorAttachmentFormats = m_ConfigInfo.colorAttachmentFormats.data();
		pipelineRenderingCreateInfo.depthAttachmentFormat = m_ConfigInfo.depthAttachmentFormat;
		pipelineRenderingCreateInfo.stencilAttachmentFormat = m_ConfigInfo.stencilAttachmentFormat;
		pipelineRenderingCreateInfo.viewMask = 0;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = &pipelineRenderingCreateInfo;
		pipelineInfo.flags = 0;
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
		pipelineInfo.renderPass = VK_NULL_HANDLE;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(VulkanContext::GetDevice()->device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_VulkanPipeline) != VK_SUCCESS)
		{
			VT_CORE_ASSERT(false, "Failed to create Graphics Pipeline")
		}

		vkDestroyShaderModule(VulkanContext::GetDevice()->device(), vertModule, nullptr);
		vkDestroyShaderModule(VulkanContext::GetDevice()->device(), fragModule, nullptr);
	}

	void VulkanPipeline::Bind(VkCommandBuffer& commandBuffer)
	{
		VT_PROFILE_FUNCTION();
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VulkanPipeline);
	}

	void VulkanPipeline::SetDefaultVulkanPipelineConfigInfo(VulkanPipelineConfigInfo* pConfigInfo)
	{
		VT_PROFILE_FUNCTION();

		// Set default color and depth formats for dynamic rendering
		pConfigInfo->colorAttachmentFormats = { VK_FORMAT_B8G8R8A8_SRGB }; // Common swapchain format
		pConfigInfo->depthAttachmentFormat = VK_FORMAT_D32_SFLOAT; // Or VK_FORMAT_D24_UNORM_S8_UINT
		pConfigInfo->stencilAttachmentFormat = VK_FORMAT_UNDEFINED; // Set to depth format if using combined depth-stencil

		// Input Assembly
		pConfigInfo->inputAssemblyInfo = {};
		pConfigInfo->inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pConfigInfo->inputAssemblyInfo.pNext = nullptr;
		pConfigInfo->inputAssemblyInfo.flags = 0;
		pConfigInfo->inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		pConfigInfo->inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		// Viewport 
		pConfigInfo->viewportInfo = {};
		pConfigInfo->viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pConfigInfo->viewportInfo.pNext = nullptr;
		pConfigInfo->viewportInfo.flags = 0;
		pConfigInfo->viewportInfo.viewportCount = 1;
		pConfigInfo->viewportInfo.scissorCount = 1;
		pConfigInfo->viewportInfo.pViewports = nullptr;
		pConfigInfo->viewportInfo.pScissors = nullptr;

		// Rasterization Info
		pConfigInfo->rasterizationInfo = {};
		pConfigInfo->rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		pConfigInfo->rasterizationInfo.pNext = nullptr;
		pConfigInfo->rasterizationInfo.flags = 0;
		pConfigInfo->rasterizationInfo.depthClampEnable = VK_FALSE;
		pConfigInfo->rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		pConfigInfo->rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		pConfigInfo->rasterizationInfo.lineWidth = 1.0f;
		pConfigInfo->rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		pConfigInfo->rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		pConfigInfo->rasterizationInfo.depthBiasEnable = VK_FALSE;
		pConfigInfo->rasterizationInfo.depthBiasConstantFactor = 0.0f;
		pConfigInfo->rasterizationInfo.depthBiasClamp = 0.0f;
		pConfigInfo->rasterizationInfo.depthBiasSlopeFactor = 0.0f;

		// Multisampling Info
		pConfigInfo->multisampleInfo = {};
		pConfigInfo->multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pConfigInfo->multisampleInfo.pNext = nullptr;
		pConfigInfo->multisampleInfo.flags = 0;
		pConfigInfo->multisampleInfo.sampleShadingEnable = VK_FALSE;
		pConfigInfo->multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		pConfigInfo->multisampleInfo.minSampleShading = 1.0f;
		pConfigInfo->multisampleInfo.pSampleMask = nullptr;
		pConfigInfo->multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		pConfigInfo->multisampleInfo.alphaToOneEnable = VK_FALSE;

		// Color Blend Attachment
		pConfigInfo->colorBlendAttachment = {};
		pConfigInfo->colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT |
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		pConfigInfo->colorBlendAttachment.blendEnable = VK_FALSE;
		pConfigInfo->colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		pConfigInfo->colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		pConfigInfo->colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		pConfigInfo->colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		pConfigInfo->colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		pConfigInfo->colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		// Color Blending Info
		pConfigInfo->colorBlendInfo = {};
		pConfigInfo->colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pConfigInfo->colorBlendInfo.pNext = nullptr;
		pConfigInfo->colorBlendInfo.flags = 0;
		pConfigInfo->colorBlendInfo.logicOpEnable = VK_FALSE;
		pConfigInfo->colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		pConfigInfo->colorBlendInfo.attachmentCount = 1;
		pConfigInfo->colorBlendInfo.pAttachments = &pConfigInfo->colorBlendAttachment;
		pConfigInfo->colorBlendInfo.blendConstants[0] = 0.0f;
		pConfigInfo->colorBlendInfo.blendConstants[1] = 0.0f;
		pConfigInfo->colorBlendInfo.blendConstants[2] = 0.0f;
		pConfigInfo->colorBlendInfo.blendConstants[3] = 0.0f;

		// Depth Stencil Info
		pConfigInfo->depthStencilInfo = {};
		pConfigInfo->depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		pConfigInfo->depthStencilInfo.pNext = nullptr;
		pConfigInfo->depthStencilInfo.flags = 0;
		pConfigInfo->depthStencilInfo.depthTestEnable = VK_FALSE;
		pConfigInfo->depthStencilInfo.depthWriteEnable = VK_FALSE;
		pConfigInfo->depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		pConfigInfo->depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		pConfigInfo->depthStencilInfo.minDepthBounds = 0.0f;
		pConfigInfo->depthStencilInfo.maxDepthBounds = 1.0f;
		pConfigInfo->depthStencilInfo.stencilTestEnable = VK_FALSE;
		pConfigInfo->depthStencilInfo.front = {};
		pConfigInfo->depthStencilInfo.back = {};

		// Dynamic States
		pConfigInfo->dynamicStatesEnable = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
		pConfigInfo->dynamicStateInfo = {};
		pConfigInfo->dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		pConfigInfo->dynamicStateInfo.pNext = nullptr;
		pConfigInfo->dynamicStateInfo.flags = 0;
		pConfigInfo->dynamicStateInfo.pDynamicStates = pConfigInfo->dynamicStatesEnable.data();
		pConfigInfo->dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(pConfigInfo->dynamicStatesEnable.size());
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
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VT_VK_CHECK(
			vkCreateShaderModule(VulkanContext::GetDevice()->device(), &createInfo, nullptr, shaderModule),
			"Failed to create shader module!"
		);
	}



}