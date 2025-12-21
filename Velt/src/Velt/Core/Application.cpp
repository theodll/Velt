#include "vtpch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Log.h"
#include <SDL3/SDL.h>
#include "ImGui/ImGuiLayer.h"
#include "Events/ApplicationEvent.h" 
#include <cassert>


bool Velt::Application::s_ShutdownRequested = false;


namespace Velt {

	Application::Application()
	{
		VT_PROFILE_FUNCTION();
		s_Instance = this;

		m_WindowProps.m_Width = WIDTH;
		m_WindowProps.m_Height = HEIGHT;
		m_WindowProps.m_Title = TITLE;
		
		m_Window = std::unique_ptr<Window>(Window::Create(m_WindowProps));
	}

	Application::~Application()
	{
		VT_PROFILE_FUNCTION();
	}

	void Application::OnEvent(Event& e)
	{
		VT_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		VT_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);

		if (auto imgui = dynamic_cast<ImGuiLayer*>(layer))
			m_ImGuiLayer = imgui;
	}

	void Application::PushOverlay(Layer* layer)
	{
		VT_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
		if (auto imgui = dynamic_cast<ImGuiLayer*>(layer))
			m_ImGuiLayer = imgui;
	}

	void Application::RenderImGui()
	{
		VT_PROFILE_FUNCTION();

		m_ImGuiLayer->Begin();
		{
			VT_PROFILE_SCOPE("LayerStack OnImGuiRender");

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
		}
		m_ImGuiLayer->End();
	}


	void Application::Run()
	{
		VT_PROFILE_FUNCTION();
		bool running = true;
		while (running)
		{
			
			VT_PROFILE_SCOPE("Application::Run Loop");
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{

				VT_PROFILE_SCOPE("SDL PollEvent Loop");

				// TODO: dont pass raw sdl events

				if (m_ImGuiLayer)
					ImGuiLayer::ProcessSDLEvent(&event);
				switch (event.type)
				{
				case SDL_EVENT_QUIT:
					{
						VT_PROFILE_SCOPE("WindowClose Event");
						Renderer::Renderer::requestShutdown();
						WindowCloseEvent e; 
						OnEvent(e); 
					}
					break;
				case SDL_EVENT_WINDOW_RESIZED:
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
					{
						VT_PROFILE_SCOPE("WindowResize Event");
						u16 w = (u16)event.window.data1;
						u16 h = (u16)event.window.data2;
						WindowResizeEvent e(w, h);
						OnEvent(e);
					}
					break;
				default:
					break;
				}
			}

			VT_PROFILE_SCOPE("Layer OnUpdate Loop");
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			RenderImGui();

			VT_PROFILE_SCOPE("Renderer drawFrame");


			if (s_ShutdownRequested)
			{
				Shutdown();
				Velt::Log::Flush();
				break;

			}
		}
	}

	void Application::Shutdown()
	{
		VT_PROFILE_FUNCTION();
	}
}
