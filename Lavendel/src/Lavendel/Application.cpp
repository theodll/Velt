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
		m_Renderer = std::make_shared<RenderAPI::Renderer>(m_Window);
		
		// Pass the layer stack to the renderer so it can iterate through layers
		// and call their OnRender methods in the correct order during rendering.
		// This ensures ImGui (and other rendering layers) respects the layer stack
		// and renders on top of scene geometry.
		m_Renderer->setLayerStack(&m_LayerStack);
	}

	Application::~Application()
	{
	
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		// dispatcher.Dispatch<WindowCloseEvent>([](WindowCloseEvent& e) { return true; });
		//LV_CORE_TRACE("{0}", e.ToString());

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}



	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);

		// If the pushed layer is an ImGuiLayer, remember it so we can forward
		// raw SDL events directly to ImGui (avoids including imgui backend headers
		// in multiple translation units).
		if (auto imgui = dynamic_cast<ImGuiLayer*>(layer))
			m_ImGuiLayer = imgui;
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		if (auto imgui = dynamic_cast<ImGuiLayer*>(layer))
			m_ImGuiLayer = imgui;
	}

	void Lavendel::Application::Run()
	{
		bool running = true;
		while (running)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
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
						WindowCloseEvent e; // defined in Events/ApplicationEvent.h
						OnEvent(e); // dispatch to layers
					}
					break;
				case SDL_EVENT_WINDOW_RESIZED:
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
					// Window size changed; notify layers with width/height
					{
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

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_Renderer->drawFrame();
		}
	}

}
