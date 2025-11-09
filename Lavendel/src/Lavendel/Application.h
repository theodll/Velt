#pragma once
#include "Core.h"
#include "Renderer/Window.h"
#include "Renderer/Core/Device.h"
#include "Renderer/Pipeline/Pipeline.h"
#include "Renderer/Core/Swapchain.h"
#include "Renderer/Renderer.h" 
#include "Log.h"
// std
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

namespace Lavendel
{	
	namespace RenderAPI {
		class Renderer;
	}
		
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

			static RenderAPI::Window& getWindow() { return m_Window; };


	private:
		inline static RenderAPI::Window m_Window{ 1280, 720, "Lavendel Window", false };
		RenderAPI::Renderer* m_Renderer;
	};

	// MUST BE DEFINED IN CLIENT

	Application* CreateApplication();

}
