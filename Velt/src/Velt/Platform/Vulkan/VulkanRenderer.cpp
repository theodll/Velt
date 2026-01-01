#include "vtpch.h"
#include "VulkanRenderer.h"
#include "VulkanContext.h"
#include "VulkanCommandBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanVertexBuffer.h"
#include "Velt/Renderer/VertexBuffer.h"
#include "Velt/Renderer/IndexBuffer.h"
#include "Velt/Renderer/Buffer.h"

namespace Velt::Renderer::Vulkan 
{
	struct SimplePushConstantData
	{
		glm::mat2 transform{1.0f};
		glm::vec2 offset;
		alignas(16) glm::vec4 color;
	};

	VulkanRenderer::VulkanRenderer()
		: m_Device(VulkanContext::GetDevice())
	{
		CreatePipelineLayout();
	}

	VulkanRenderer::~VulkanRenderer()
	{
		VT_PROFILE_FUNCTION();
		
		auto device = m_Device.device();
		
		if (m_PipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
		}
		
		FreeCommandBuffers();
	}

	void VulkanRenderer::Init(VulkanSwapchain& swapchain)
	{
		VT_PROFILE_FUNCTION();
		
		m_Swapchain = &swapchain;
		CreatePipeline();
		CreateVertexBuffer();
		CreateCommandBuffers();
	}

	void VulkanRenderer::DrawFrame(VulkanSwapchain& swapchain)
	{
		VT_PROFILE_FUNCTION();
		
		u32 result = swapchain.AcquireNextImage();
		
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			return;
		}
		
		u32 imageIndex = result;
		RecordCommandBuffer(imageIndex, swapchain);
		
		u32 frameIndex = imageIndex % VulkanSwapchain::MAX_FRAMES_IN_FLIGHT;
		VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];
		
		swapchain.SubmitCommandBuffers(&commandBuffer, &imageIndex);
	}

	void VulkanRenderer::CreatePipelineLayout()
	{
		VT_PROFILE_FUNCTION();
		
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
		{
			VT_CORE_ERROR("Failed to create pipeline layout!");
		}
	}

	void VulkanRenderer::CreatePipeline()
	{
		VT_PROFILE_FUNCTION();

		PipelineSpecification spec;
		spec.DebugName = "TrianglePipeline";
		spec.VertexShaderPath = "shaders/shader.vert.spv";
		spec.FragmentShaderPath = "shaders/shader.frag.spv";
		
		BufferLayout layout = {
			{ShaderDataType::Float2, "position"},
			{ShaderDataType::Float3, "color"}
		};
		spec.Layout = layout;
		
		m_Pipeline = std::unique_ptr<VulkanPipeline>(new VulkanPipeline(spec));
		
		// Set up pipeline config with render pass and layout
		VulkanPipelineConfigInfo& configInfo = m_Pipeline->GetConfigInfo();
		configInfo.pipelineLayout = m_PipelineLayout;
		configInfo.renderPass = m_Swapchain->GetRenderPass();
		
		m_Pipeline->Invalidate();
	}

	void VulkanRenderer::CreateVertexBuffer()
	{
		VT_PROFILE_FUNCTION();
		
		std::vector<Vertex> vertices = {
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size(), sizeof(Vertex));
	}

	void VulkanRenderer::CreateCommandBuffers()
	{
		VT_PROFILE_FUNCTION();
		
		m_CommandBuffers.resize(VulkanSwapchain::MAX_FRAMES_IN_FLIGHT);
		
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = m_Device.getCommandPool();
		allocInfo.commandBufferCount = static_cast<u32>(m_CommandBuffers.size());

		if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		{
			VT_CORE_ERROR("Failed to allocate command buffers!");
		}
	}

	void VulkanRenderer::FreeCommandBuffers()
	{
		VT_PROFILE_FUNCTION();
		
		if (!m_CommandBuffers.empty())
		{
			vkFreeCommandBuffers(
				m_Device.device(),
				m_Device.getCommandPool(),
				static_cast<u32>(m_CommandBuffers.size()),
				m_CommandBuffers.data()
			);
			m_CommandBuffers.clear();
		}
	}

	void VulkanRenderer::RecordCommandBuffer(u32 imageIndex, VulkanSwapchain& swapchain)
	{
		VT_PROFILE_FUNCTION();
		
		u32 frameIndex = imageIndex % VulkanSwapchain::MAX_FRAMES_IN_FLIGHT;
		VkCommandBuffer commandBuffer = m_CommandBuffers[frameIndex];

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			VT_CORE_ERROR("Failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapchain.GetRenderPass();
		renderPassInfo.framebuffer = swapchain.GetFrameBuffer(imageIndex);
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = {swapchain.GetWidth(), swapchain.GetHeight()};

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
		clearValues[1].depthStencil = {1.0f, 0};
		renderPassInfo.clearValueCount = static_cast<u32>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapchain.GetWidth());
		viewport.height = static_cast<float>(swapchain.GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = {swapchain.GetWidth(), swapchain.GetHeight()};
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		m_Pipeline->Bind(commandBuffer);
		
		// Bind vertex buffer
		auto vulkanVertexBuffer = std::static_pointer_cast<VulkanVertexBuffer>(m_VertexBuffer);
		VkBuffer vertexBuffers[] = {vulkanVertexBuffer->GetBuffer()};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		SimplePushConstantData push{};
		push.offset = {0.0f, 0.0f};
		push.color = {1.0f, 1.0f, 1.0f, 1.0f};

		vkCmdPushConstants(
			commandBuffer,
			m_PipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push
		);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			VT_CORE_ERROR("Failed to record command buffer!");
		}
	}
}