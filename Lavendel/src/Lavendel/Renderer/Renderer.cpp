#include "lvpch.h"
#include "Renderer.h"
#include "Pipeline/Pipeline.h"
#include "Lavendel/ImGui/ImGuiLayer.h"
#include "Lavendel/Layers/LayerStack.h"
#include "Lavendel/Layers/Layer.h"

// Define static members declared in Renderer.h. These must have a single
// definition in a .cpp file to satisfy the linker. They are used to provide
// global-ish access to the GPU device, swapchain and pipeline owned by the
// renderer instance.
std::shared_ptr<Lavendel::RenderAPI::GPUDevice> Lavendel::RenderAPI::Renderer::m_Device = nullptr;
std::shared_ptr<Lavendel::RenderAPI::SwapChain> Lavendel::RenderAPI::Renderer::m_SwapChain = nullptr;
std::shared_ptr<Lavendel::RenderAPI::Pipeline> Lavendel::RenderAPI::Renderer::m_Pipeline = nullptr;

namespace Lavendel {
	namespace RenderAPI {

		Renderer::Renderer(Window& window) : m_Window(window)
		{
			LV_CORE_INFO("Initializing Renderer...");

			m_Device = std::make_shared<GPUDevice>(m_Window);
			loadModels();
			createPipelineLayout();
			recreateSwapChain();
			createCommandBuffers();
		}

		Renderer::~Renderer()
		{
			vkDestroyPipelineLayout(m_Device->device(), m_PipelineLayout, nullptr);
		}

		void Renderer::createPipelineLayout()
		{
			VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0;
			pipelineLayoutInfo.pSetLayouts = nullptr;
			pipelineLayoutInfo.pushConstantRangeCount = 0;
			pipelineLayoutInfo.pPushConstantRanges = nullptr;
			if (vkCreatePipelineLayout(m_Device->device(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
			{
				LV_CORE_ERROR("Failed to create pipeline layout!");
				throw std::runtime_error("Failed to create pipeline layout!");
			}
		}

		void Renderer::createPipeline()
		{
			PipelineConfigInfo pipelineConfig{};
			Lavendel::RenderAPI::Pipeline::defaultPipelineConfigInfo(pipelineConfig);
			pipelineConfig.renderPass = m_SwapChain->getRenderPass();
			pipelineConfig.pipelineLayout = m_PipelineLayout;
			m_Pipeline = std::make_shared<Lavendel::RenderAPI::Pipeline>(
				*m_Device,
				"shaders/shader.vert.spv",
				"shaders/shader.frag.spv",
				pipelineConfig);
		}

		void Renderer::recreateSwapChain()
		{
			auto extent = m_Window.getExtent();
			while (extent.width == 0 || extent.height == 0)
			{
				extent = m_Window.getExtent();
				SDL_WaitEvent(nullptr);
			}

			vkDeviceWaitIdle(m_Device->device());

			if (m_SwapChain == nullptr)
			{
				m_SwapChain = std::make_shared<SwapChain>(*m_Device, extent);
			}
			else
			{
				std::shared_ptr<SwapChain> oldSwapChain = std::move(m_SwapChain);
				m_SwapChain = std::make_shared<SwapChain>(*m_Device, extent, oldSwapChain);

				if (m_SwapChain->imageCount() != m_CommandBuffers.size())
				{
					freeCommandBuffers(); 
					createCommandBuffers();
				}
			}
			createPipeline();
		}

		void Renderer::loadModels()
		{
			std::vector<Model::Vertex> vertices = {
				{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
				{{ -0.5f, 0.5f }, {0.0f, 1.0f, 0.0f}},
				{{0.5f, 0.5f},  {0.0f, 0.0f, 1.0f}}
			};
			m_Model = std::make_shared<Model>(*m_Device, vertices);
		}

		void Renderer::createCommandBuffers()
		{
			m_CommandBuffers.resize(m_SwapChain->imageCount());

			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = m_Device->getCommandPool();
			allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

			if (vkAllocateCommandBuffers(m_Device->device(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
			{
				LV_CORE_ERROR("Failed to allocate command buffers!");
				throw std::runtime_error("Failed to allocate command buffers!");
			}
		}

		void Renderer::freeCommandBuffers()
		{
			vkFreeCommandBuffers(
				m_Device->device(),
				m_Device->getCommandPool(),
				static_cast<uint32_t>(m_CommandBuffers.size()),
				m_CommandBuffers.data());
			m_CommandBuffers.clear();
		}

		void Renderer::recordCommandBuffer(int imageIndex)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			if (vkBeginCommandBuffer(m_CommandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
			{
				LV_CORE_ERROR("Failed to begin recording command buffer!");
				throw std::runtime_error("Failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_SwapChain->getRenderPass();
			renderPassInfo.framebuffer = m_SwapChain->getFrameBuffer(imageIndex);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = m_SwapChain->getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { {0.01f, 0.01f, 0.01f, 1.0f} };
			clearValues[1].depthStencil = { 1.0f, 0 };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(m_CommandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(m_SwapChain->getSwapChainExtent().width);
			viewport.height = static_cast<float>(m_SwapChain->getSwapChainExtent().height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			VkRect2D scissor{ {0, 0}, m_SwapChain->getSwapChainExtent()};
			vkCmdSetViewport(m_CommandBuffers[imageIndex], 0, 1, &viewport);
			vkCmdSetScissor(m_CommandBuffers[imageIndex], 0, 1, &scissor);

			// Render scene - bind pipeline and draw the model
			m_Pipeline->bind(m_CommandBuffers[imageIndex]);
			m_Model->bind(m_CommandBuffers[imageIndex]);
			m_Model->draw(m_CommandBuffers[imageIndex]);

			// Render layers in stack order. This respects the layer stack and ensures
			// that ImGui (typically added as the last layer via PushLayer) renders on top.
			// Each layer's OnRender() method is called in order, allowing layers like ImGuiLayer
			// to render their graphics to the command buffer at the appropriate depth.
			if (m_LayerStack != nullptr)
			{
				for (Layer* layer : *m_LayerStack)
				{
					// Pass the command buffer as a void* to avoid including Vulkan headers in Layer.h
					layer->OnRender(reinterpret_cast<void*>(m_CommandBuffers[imageIndex]));
				}
			}

			vkCmdEndRenderPass(m_CommandBuffers[imageIndex]);
			if (vkEndCommandBuffer(m_CommandBuffers[imageIndex]) != VK_SUCCESS)
			{
				LV_CORE_ERROR("Failed to record command buffer!");
				throw std::runtime_error("Failed to record command buffer!");
			}
		}

		void Renderer::drawFrame()
		{
			uint32_t imageIndex;
			auto result = m_SwapChain->acquireNextImage(&imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				recreateSwapChain();
				return;
			}

			if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				LV_CORE_ERROR("Failed to acquire swap chain image!");
				throw std::runtime_error("Failed to acquire swap chain image!");
			}

			recordCommandBuffer(imageIndex);

			result = m_SwapChain->submitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_Window.wasWindodwResized())
			{
				m_Window.resetWindowResizedFlag();
				recreateSwapChain();
				return;
			}
			else if (result != VK_SUCCESS)
			{
				LV_CORE_ERROR("Failed to present swap chain image!");
				throw std::runtime_error("Failed to present swap chain image!");
			}
		}

		void Renderer::renderImGui(VkCommandBuffer commandBuffer)
		{
			// This method is called during recordCommandBuffer when rendering the ImGuiLayer.
			// By separating ImGui rendering into its own method and calling it from the layer
			// iteration process, we ensure ImGui respects the layer stack order and renders
			// on top of previous layers (like the scene triangle).
			
			if (m_ImGuiLayer != nullptr)
			{
				// Render the ImGui draw data that was prepared during ImGuiLayer::OnUpdate()
				m_ImGuiLayer->GetRenderer().Render(commandBuffer);
			}
		}

	}  // namespace RenderAPI
}  // namespace Lavendel