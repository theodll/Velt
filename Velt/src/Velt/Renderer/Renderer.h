#pragma once
#include "vtpch.h"
#include "Window.h"
#include "Platorm/Vulkan/Device.h"
#include "Pipeline/Pipeline.h"
#include "Core/Swapchain.h"
#include "Model/Model.h"

namespace Velt::Renderer
{
	// Forward declaration
	class Application;
	class ImGuiLayer;
	class LayerStack;

	namespace Velt::Render
	{
		class VELT_API Renderer
		{
		public:
			Renderer(Window &window);
			~Renderer();

			Renderer(const Window &) = delete;
			Renderer &operator=(const Window &) = delete;

			void drawFrame();

			void setImGuiLayer(ImGuiLayer *layer) { m_ImGuiLayer = layer; }
			void setLayerStack(LayerStack *layerStack) { m_LayerStack = layerStack; }
			void renderImGui(VkCommandBuffer commandBuffer);
			static void requestShutdown();
			static void Shutdown();

			inline static VulkanDevice *getDevice() { return m_Device; }
			inline static SwapChain *getSwapChain() { return m_SwapChain; }
			inline static Pipeline *getPipeline() { return m_Pipeline; }

		private:
			void loadModels();
			void createPipelineLayout();
			void createPipeline();
			void createCommandBuffers();
			void freeCommandBuffers();
			void recreateSwapChain();
			void recordCommandBuffer(int imageIndex);

			Window &m_Window;
			static GPUDevice *m_Device;
			static SwapChain *m_SwapChain;
			static Pipeline *m_Pipeline;
			std::shared_ptr<Model> m_Model;
			std::vector<VkCommandBuffer> m_CommandBuffers;
			VkPipelineLayout m_PipelineLayout;
			ImGuiLayer *m_ImGuiLayer = nullptr;
			LayerStack *m_LayerStack = nullptr; // Reference to layer stack for calling OnRender on each layer
		};
	}
}
