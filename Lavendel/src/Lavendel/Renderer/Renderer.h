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

	namespace RenderAPI
	{
		class LAVENDEL_API Renderer
		{
		public:
			Renderer(Window& window);
			~Renderer();
			
			void drawFrame();

		private:
			void loadModels();
			void createPipelineLayout();
			void createPipeline();
			void createCommandBuffers();
			void freeCommandBuffers();
			void recreateSwapChain();
			void recordCommandBuffer(int imageIndex);
			


			Window& m_Window;
			std::shared_ptr<GPUDevice> m_Device;
			std::shared_ptr<SwapChain> m_SwapChain;
			std::shared_ptr<RenderAPI::Pipeline> m_Pipeline;
			std::shared_ptr<Model> m_Model;
			std::vector<VkCommandBuffer> m_CommandBuffers;
			VkPipelineLayout m_PipelineLayout;
		};
	}
}
