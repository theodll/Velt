#pragma once
#include "Core.h"
#include "Renderer/Window.h"
#include "Renderer/Setup/Device.h"
#include "Renderer/Setup/Pipeline.h"
#include <string>


namespace Lavendel
{

	class LAVENDEL_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();

			static constexpr int WIDTH = 1280;
			static constexpr int HEIGHT = 1280;
			std::string TITLE{ "Lavendel Engine" };

	private:

		Lavendel::RendererAPI::Window m_Window{ 1280, 720, "Lavendel Window", false };
		Lavendel::RendererAPI::GPUDevice m_Device{ m_Window };
		Lavendel::RendererAPI::Pipeline m_Pipeline{m_Device, "shaders/shader.vert.spv", "shaders/shader.frag.spv", Lavendel::RendererAPI::Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
		};
	};

	// MUST BE DEFINED IN CLIENT

	Application* CreateApplication();

}
