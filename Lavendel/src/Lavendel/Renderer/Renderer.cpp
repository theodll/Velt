#include "Renderer.h"

namespace Lavendel {
	namespace RenderAPI {

		Renderer::Renderer(Window& window) : m_Window(window)
		{
			createPipelineLayout();
			createPipeline();
			createCommandBuffers();
		}

		Renderer::~Renderer()
		{
			vkDestroyPipelineLayout(m_Device.device(), m_PipelineLayout, nullptr);
		}

		void Renderer::createPipelineLayout()
		{
			VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0;
			pipelineLayoutInfo.pSetLayouts = nullptr;
			pipelineLayoutInfo.pushConstantRangeCount = 0;
			pipelineLayoutInfo.pPushConstantRanges = nullptr;
			if (vkCreatePipelineLayout(m_Device.device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
			{
				LV_CORE_ERROR("Failed to create pipeline layout!");
				throw std::runtime_error("Failed to create pipeline layout!");
			}
		}

		void Renderer::createPipeline()
		{
			auto pipelineConfig = Lavendel::RenderAPI::Pipeline::defaultPipelineConfigInfo(m_SwapChain.width(), m_SwapChain.height());
			pipelineConfig.renderPass = m_SwapChain.getRenderPass();
			pipelineConfig.pipelineLayout = m_PipelineLayout;
			m_Pipeline = std::make_unique<Lavendel::RenderAPI::Pipeline>(
				m_Device,
				"shaders/shader.vert.spv",
				"shaders/shader.frag.spv",
				pipelineConfig);
		}

		void Renderer::createCommandBuffers()
		{
			m_CommandBuffers.resize(m_SwapChain.imageCount());

			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = m_Device.getCommandPool();
			allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

			if (vkAllocateCommandBuffers(m_Device.device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
			{
				LV_CORE_ERROR("Failed to allocate command buffers!");
				throw std::runtime_error("Failed to allocate command buffers!");
			}

			for (int i = 0; i < m_CommandBuffers.size(); i++)
			{
				VkCommandBufferBeginInfo beginInfo{};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				if (vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS)
				{
					LV_CORE_ERROR("Failed to begin recording command buffer!");
					throw std::runtime_error("Failed to begin recording command buffer!");
				}

				VkRenderPassBeginInfo renderPassInfo{};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = m_SwapChain.getRenderPass();
				renderPassInfo.framebuffer = m_SwapChain.getFrameBuffer(i);

				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = m_SwapChain.getSwapChainExtent();

				std::array<VkClearValue, 2> clearValues{};
				clearValues[0].color = { {0.01f, 0.01f, 0.01f, 1.0f} };
				clearValues[1].depthStencil = { 1.0f, 0 };
				renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
				renderPassInfo.pClearValues = clearValues.data();

				vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				m_Pipeline->bind(m_CommandBuffers[i]);
				vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

				vkCmdEndRenderPass(m_CommandBuffers[i]);
				if (vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
				{
					LV_CORE_ERROR("Failed to record command buffer!");
					throw std::runtime_error("Failed to record command buffer!");
				}
			}
		}

		void Renderer::drawFrame()
		{
			uint32_t imageIndex;
			auto result = m_SwapChain.acquireNextImage(&imageIndex);
			if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				LV_CORE_ERROR("Failed to acquire swap chain image!");
				throw std::runtime_error("Failed to acquire swap chain image!");
			}

			result = m_SwapChain.submitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);
			if (result != VK_SUCCESS)
			{
				LV_CORE_ERROR("Failed to present swap chain image!");
				throw std::runtime_error("Failed to present swap chain image!");
			}
		}
	}  // namespace RenderAPI
}