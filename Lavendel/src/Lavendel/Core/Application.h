#pragma once
#include "lvpch.h"

#include "Renderer/Window.h"
#include "Renderer/Renderer.h"
#include "Layers/LayerStack.h"
#include "ImGui/ImGuiLayer.h"

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
			void Shutdown();

			void OnEvent(Event& e);
			void operator=(const Application&) = delete;

			static constexpr int WIDTH = 1280;
			static constexpr int HEIGHT = 1280;
			std::string TITLE{ "Lavendel Engine" };

			static RenderAPI::Window& getWindow() { return m_Window; };
			static bool s_ShutdownRequested;
		
			void RenderImGui();

			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);

	private:
		inline static RenderAPI::Window m_Window{ 800, 600, "Lavendel Window", true };
		std::shared_ptr<RenderAPI::Renderer> m_Renderer;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer = nullptr;
	};

	// MUST BE DEFINED IN CLIENT

	Application* CreateApplication();

}
