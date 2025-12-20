#pragma once
#include "vtpch.h"

#include "Window.h"
#include "Layers/LayerStack.h"
#include "ImGui/ImGuiLayer.h"

namespace Velt
{	
	namespace RenderAPI {
		class Renderer;
	}
		
	class VELT_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();
			void Shutdown();

			void OnEvent(Event& e);
			void operator=(const Application&) = delete;

			static constexpr int WIDTH = 1280;
			static constexpr int HEIGHT = 1280;
			std::string TITLE{ "Velt Engine" };

			static bool s_ShutdownRequested;
		
			void RenderImGui();

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);

	private:
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer = nullptr;
	};

	// MUST BE DEFINED IN CLIENT

	Application* CreateApplication();

}
