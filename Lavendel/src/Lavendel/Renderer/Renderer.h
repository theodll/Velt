#pragma once
#include "lvpch.h"
#include "Window.h"
#include "Core/Device.h"
#include "Pipeline/Pipeline.h"
#include "Core/Swapchain.h"
#include "Model/Model.h"


namespace Lavendel
{
	// Forward declaration
	class Application;
	class ImGuiLayer;
	class LayerStack;

	namespace RenderAPI
	{
		class LAVENDEL_API Renderer
		{
		public:
			Renderer(Window& window);
			~Renderer();
			
			void drawFrame();
			
			void setImGuiLayer(ImGuiLayer* layer) { m_ImGuiLayer = layer; }
			void setLayerStack(LayerStack* layerStack) { m_LayerStack = layerStack; }
			void renderImGui(VkCommandBuffer commandBuffer);
			void shutdown();

			inline static const std::shared_ptr<GPUDevice> getDevice() { return m_Device; }
			inline static const std::shared_ptr<SwapChain> getSwapChain()  { return m_SwapChain; }
			inline static const std::shared_ptr<Pipeline> getPipeline() { return m_Pipeline; }

		private:
			void loadModels();
			void createPipelineLayout();
			void createPipeline();
			void createCommandBuffers();
			void freeCommandBuffers();
			void recreateSwapChain();
			void recordCommandBuffer(int imageIndex);
			

			Window& m_Window;
			static std::shared_ptr<GPUDevice> m_Device;
			static std::shared_ptr<SwapChain> m_SwapChain;
			static std::shared_ptr<RenderAPI::Pipeline> m_Pipeline;
			std::shared_ptr<Model> m_Model;
			std::vector<VkCommandBuffer> m_CommandBuffers;
			VkPipelineLayout m_PipelineLayout;
			ImGuiLayer* m_ImGuiLayer = nullptr;
			LayerStack* m_LayerStack = nullptr;  // Reference to layer stack for calling OnRender on each layer
		};
	}
}
