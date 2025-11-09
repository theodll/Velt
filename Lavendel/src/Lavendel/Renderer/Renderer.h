#pragma once

#include "../Core.h"
#include "Window.h"
#include "Core/Device.h"
#include "Pipeline/Pipeline.h"
#include "Core/Swapchain.h"
#include "../Log.h"

// std
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

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

			Renderer(const Renderer&) = delete;
			Renderer& operator=(const Renderer&) = delete;
			
			void drawFrame();

		private:
			void createPipelineLayout();
			void createPipeline();
			void createCommandBuffers();

			Window& m_Window;
			GPUDevice m_Device{ m_Window };
			SwapChain m_SwapChain{ m_Device, m_Window.getExtent() };
			std::unique_ptr<RenderAPI::Pipeline> m_Pipeline;
			VkPipelineLayout m_PipelineLayout;
			std::vector<VkCommandBuffer> m_CommandBuffers;
		};
	}
}


