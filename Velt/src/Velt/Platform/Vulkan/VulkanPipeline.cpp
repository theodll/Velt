#include "vtpch.h"
#include "Core/Core.h"
#include "VulkanPipeline.h"
#include "Renderer/PipelineManager.h"
#include "Renderer/BindingLayouts.h"
#include "VulkanContext.h"
#include "Core/Application.h"
#include <algorithm>

namespace Velt::RHI {

	PipelineSpecification VulkanPipeline::m_Specification;

	VulkanPipeline::~VulkanPipeline()
	{

	}

	VulkanPipeline::VulkanPipeline(const PipelineSpecification* pSpecs)
		: m_Layout(pSpecs->Layout)
	{
		m_Specification = *pSpecs;
		VT_PROFILE_FUNCTION();
	}

	void VulkanPipeline::Init()
	{
		VT_PROFILE_FUNCTION();
		SetDefaultVulkanPipelineConfigInfo(&m_ConfigInfo);
		CreatePipelineLayout();
		Invalidate();
	}

	void VulkanPipeline::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("Shutdown Vulkan Pipeline");
		auto& device = VulkanContext::GetDevice()->device();

		for (auto& setLayout : m_Specification.SetLayouts)
			if (setLayout != VT_NULL_HANDLE)
				vkDestroyDescriptorSetLayout(device, setLayout, VT_NULL_HANDLE);

		vkDestroyPipeline(device, m_VulkanPipeline, nullptr);
		vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
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
		default: return VK_FORMAT_UNDEFINED;
		}
	}

	void VulkanPipeline::CreatePipelineLayout()
	{
		VT_PROFILE_FUNCTION();
		if (m_Specification.SetLayouts.empty())
		{
			std::vector<std::vector<DescriptorBinding>> setBindings;
			
			// Set 0  Global bindings
			setBindings.push_back(GetGlobalSetBindings());
			
			// Set 1  Material bindings
			setBindings.push_back(GetMaterialSetBindings());
			
			// Set 2  Deferred bindings
			setBindings.push_back(GetDefferedSetBindings());

			m_Specification.SetLayouts.resize(setBindings.size());
			for (u32 setNumber = 0; setNumber < setBindings.size(); setNumber++)
			{
				auto& bindings = setBindings[setNumber];
				std::sort(bindings.begin(), bindings.end(), [](const DescriptorBinding& a, const DescriptorBinding& b)
				{
					return a.binding < b.binding;
				});
				m_Specification.SetLayouts[setNumber] = VulkanContext::GetLayoutCache()->CreateLayout(&bindings);
			}
		}

		struct PushConstants 
		{
			glm::mat4 transform;
			u32 entityID;
		};

		VkPushConstantRange range = {};
		range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		range.offset = 0;
		range.size = sizeof(PushConstants);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = (u32)m_Specification.SetLayouts.size();
		pipelineLayoutInfo.pSetLayouts = m_Specification.SetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &range;

		if (vkCreatePipelineLayout(VulkanContext::GetDevice()->device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
			VT_CORE_ASSERT(false, "Failed to create pipeline layout");

		m_ConfigInfo.pipelineLayout = m_PipelineLayout;
	}

	void VulkanPipeline::Invalidate()
	{
		VkPipelineShaderStageCreateInfo shaderStages[2] = {};
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = m_Specification.VertexShader->Module;
		shaderStages[0].pName = "main";

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = m_Specification.FragmentShader->Module;
		shaderStages[1].pName = "main";

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
					attr.offset = element.Offset + (i * 16);
					attributeDescs.push_back(attr);
				}
			}
			else
			{
				VkVertexInputAttributeDescription attr{};
				attr.binding = 0;
				attr.location = location++;
				attr.format = ShaderDataTypeToVulkanFormat(element.Type);
				attr.offset = element.Offset;
				attributeDescs.push_back(attr);
			}
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDesc;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescs.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescs.data();

		VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo{};
		pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		pipelineRenderingCreateInfo.colorAttachmentCount = static_cast<uint32_t>(m_ConfigInfo.colorAttachmentFormats.size());
		pipelineRenderingCreateInfo.pColorAttachmentFormats = m_ConfigInfo.colorAttachmentFormats.data();
		pipelineRenderingCreateInfo.depthAttachmentFormat = m_ConfigInfo.depthAttachmentFormat;
		pipelineRenderingCreateInfo.stencilAttachmentFormat = m_ConfigInfo.stencilAttachmentFormat;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = &pipelineRenderingCreateInfo;
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

		if (vkCreateGraphicsPipelines(VulkanContext::GetDevice()->device(), PipelineManager::GetVulkanCache(), 1, &pipelineInfo, nullptr, &m_VulkanPipeline) != VK_SUCCESS)
			VT_CORE_ASSERT(false, "Failed to create Graphics Pipeline")

			vkDestroyShaderModule(VulkanContext::GetDevice()->device(), m_Specification.VertexShader->Module, nullptr);
		vkDestroyShaderModule(VulkanContext::GetDevice()->device(), m_Specification.FragmentShader->Module, nullptr);
	}

	void VulkanPipeline::SetDefaultVulkanPipelineConfigInfo(VulkanPipelineConfigInfo* pConfigInfo)
	{
		VT_PROFILE_FUNCTION();

		pConfigInfo->colorAttachmentFormats = m_Specification.ColorAttachmentFormats;
		pConfigInfo->depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;
		pConfigInfo->stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

		pConfigInfo->inputAssemblyInfo = {};
		pConfigInfo->inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pConfigInfo->inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		pConfigInfo->viewportInfo = {};
		pConfigInfo->viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pConfigInfo->viewportInfo.viewportCount = 1;
		pConfigInfo->viewportInfo.scissorCount = 1;

		pConfigInfo->rasterizationInfo = {};
		pConfigInfo->rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		pConfigInfo->rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		pConfigInfo->rasterizationInfo.lineWidth = 1.0f;
		pConfigInfo->rasterizationInfo.cullMode = (VkCullModeFlagBits)m_Specification.CullMode;
		pConfigInfo->rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;

		pConfigInfo->multisampleInfo = {};
		pConfigInfo->multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pConfigInfo->multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = 0xF;
		colorBlendAttachment.blendEnable = VK_FALSE;

		pConfigInfo->colorBlendAttachments.clear();
		for (uint32_t i = 0; i < pConfigInfo->colorAttachmentFormats.size(); ++i)
			pConfigInfo->colorBlendAttachments.push_back(colorBlendAttachment);

		pConfigInfo->colorBlendInfo = {};
		pConfigInfo->colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pConfigInfo->colorBlendInfo.attachmentCount = static_cast<uint32_t>(pConfigInfo->colorBlendAttachments.size());
		pConfigInfo->colorBlendInfo.pAttachments = pConfigInfo->colorBlendAttachments.data();

		pConfigInfo->depthStencilInfo = {};
		pConfigInfo->depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		pConfigInfo->depthStencilInfo.depthTestEnable = VK_TRUE;
		pConfigInfo->depthStencilInfo.depthWriteEnable = VK_TRUE;
		pConfigInfo->depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;

		pConfigInfo->dynamicStatesEnable = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		pConfigInfo->dynamicStateInfo = {};
		pConfigInfo->dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		pConfigInfo->dynamicStateInfo.pDynamicStates = pConfigInfo->dynamicStatesEnable.data();
		pConfigInfo->dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(pConfigInfo->dynamicStatesEnable.size());
	}

	void VulkanPipeline::Bind(VkCommandBuffer& commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VulkanPipeline);
	}
}