#include "lvpch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Log.h"
#include <SDL3/SDL.h>
#include "ImGui/ImGuiLayer.h"
#include "Events/ApplicationEvent.h" // Import WindowCloseEvent / WindowResizeEvent

// CONSTRUCTOR 
namespace Lavendel {

	Application::Application()
	{
		LV_PROFILE_FUNCTION();
		m_Renderer = std::make_shared<RenderAPI::Renderer>(m_Window);
		
		m_Renderer->setLayerStack(&m_LayerStack);
	}

	Application::~Application()
	{
		LV_PROFILE_FUNCTION();
	
	}

	void Application::OnEvent(Event& e)
	{
		LV_PROFILE_FUNCTION();
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
		LV_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);

		// Todo: make events
		if (auto imgui = dynamic_cast<ImGuiLayer*>(layer))
			m_ImGuiLayer = imgui;
	}

	void Application::PushOverlay(Layer* layer)
	{
		LV_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
		if (auto imgui = dynamic_cast<ImGuiLayer*>(layer))
			m_ImGuiLayer = imgui;
	}

	void Lavendel::Application::Run()
	{
		LV_PROFILE_FUNCTION();
		bool running = true;
		while (running)
		{
			LV_PROFILE_SCOPE("Application::Run Loop");
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				LV_PROFILE_SCOPE("SDL PollEvent Loop");
				// First, give ImGui a chance to process the raw SDL event. ImGui's
				// backend will update its IO state (mouse/keyboard) and set capture
				// flags so widgets become interactive.
				if (m_ImGuiLayer)
					ImGuiLayer::ProcessSDLEvent(&event);

				// Then translate a small subset of SDL events into the engine's
				// Event classes so other layers can react (window close / resize).
				switch (event.type)
				{
				case SDL_EVENT_QUIT:
					// User requested app quit (e.g. clicking close button)
					running = false;
					{
						LV_PROFILE_SCOPE("WindowClose Event");
						WindowCloseEvent e; // defined in Events/ApplicationEvent.h
						OnEvent(e); // dispatch to layers
					}
					break;
				case SDL_EVENT_WINDOW_RESIZED:
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
					// Window size changed; notify layers with width/height
					{
						LV_PROFILE_SCOPE("WindowResize Event");
						uint16_t w = (uint16_t)event.window.data1;
						uint16_t h = (uint16_t)event.window.data2;
						WindowResizeEvent e(w, h);
						OnEvent(e);
					}
					break;
				default:
					// Other events are left for ImGui (processed above) or can be
					// translated later if you want engine-level mouse/keyboard events.
					break;
				}
			}

			LV_PROFILE_SCOPE("Layer OnUpdate Loop");
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			LV_PROFILE_SCOPE("Renderer drawFrame");
			m_Renderer->drawFrame();
		}
		Shutdown();
	}

	void Application::Shutdown()
	{
		if (RenderAPI::Renderer::getDevice) {
			vkDeviceWaitIdle(RenderAPI::Renderer::getDevice()->device());
		}

		if (m_ImGuiLayer) {
			m_ImGuiLayer = nullptr;
		}

	}

}
