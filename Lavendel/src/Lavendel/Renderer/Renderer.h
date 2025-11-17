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
			
			// Set ImGui layer for rendering
			// This is called by Application when ImGuiLayer is pushed, allowing the Renderer
			// to be aware of the ImGui layer for proper rendering order through the layer stack
			void setImGuiLayer(ImGuiLayer* layer) { m_ImGuiLayer = layer; }

			// Set the layer stack so the renderer can iterate through layers during rendering.
			// This allows layers to call their OnRender methods in the correct order,
			// ensuring ImGui (and other rendering layers) appear on top of scene geometry.
			void setLayerStack(LayerStack* layerStack) { m_LayerStack = layerStack; }

			// Render ImGui draw data to the current command buffer
			// Called during recordCommandBuffer when we process the ImGuiLayer in the layer stack
			void renderImGui(VkCommandBuffer commandBuffer);

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
