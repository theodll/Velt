#pragma once
#include "vtpch.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Window.h"
#include "Layers/LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Timestep.h"
#include "Renderer/OrthographicCamera.h"

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

			void operator=(const Application&) = delete;

			void Init();
			void Run();
			void Shutdown();
		
			void OnEvent(Event& e);
		
			static inline Application& Get() { if (s_Instance) { return *s_Instance; } else VT_CORE_ASSERT(s_Instance, "Application Instance is null.") }
		
			void RenderImGui();
			void PushLayer(Layer* layer);
			void PushOverlay(Layer* overlay);
			Window& GetWindow() { return *m_Window; }

			void RenderStatisticsWidget(Timestep ts);
		
			// VARS
			static bool s_ShutdownRequested;
			static constexpr int WIDTH {1280};
			static constexpr int HEIGHT { 720 };
			std::string TITLE { "Velt Engine" };

	private:
		// Window Things
		static Application* s_Instance;
		std::unique_ptr<Renderer::RHI::VulkanContext> m_Context;
		std::unique_ptr<Window> m_Window;
		
		// Layers
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer = nullptr;

		// Misc 
		double m_LastFrameTime;

		// Properties
		WindowProps m_WindowProps;

		
	};

	Application* CreateApplication();

}
