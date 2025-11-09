#pragma once
#include "Core.h"
#include "Renderer/Window.h"
#include "Renderer/Core/Device.h"
#include "Renderer/Pipeline/Pipeline.h"
#include "Renderer/Core/Swapchain.h"
#include "Log.h"
// std
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

namespace Lavendel
{

	class LAVENDEL_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();

			void operator=(const Application&) = delete;

			static constexpr int WIDTH = 1280;
			static constexpr int HEIGHT = 1280;
			std::string TITLE{ "Lavendel Engine" };


	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();

		RenderAPI::Window m_Window{1280, 720, "Lavendel Window", false };
		RenderAPI::GPUDevice m_Device{m_Window };
//		RenderAPI::Pipeline m_Pipeline{ m_Device, "shaders/shader.vert.spv", "shaders/shader.frag.spv", Lavendel::RenderAPI::Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT) };
		RenderAPI::SwapChain m_SwapChain{ m_Device, m_Window.getExtent() };
		std::unique_ptr<RenderAPI::Pipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout;
		std::vector<VkCommandBuffer> m_CommandBuffers;
	};

	// MUST BE DEFINED IN CLIENT

	Application* CreateApplication();

}
