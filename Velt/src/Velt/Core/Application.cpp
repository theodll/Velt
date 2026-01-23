#include "vtpch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Log.h"
#include <SDL3/SDL.h>
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Buffer.h"
#include "Events/ApplicationEvent.h" 
#include <cassert>


bool Velt::Application::s_ShutdownRequested = false;


Velt::Application* Velt::Application::s_Instance;

namespace Velt {



	VkFormat ShaderDataTypeToVulkanBaseType(Renderer::ShaderDataType type) {
		switch(type) {
			case Renderer::ShaderDataType::Float:   return VK_FORMAT_R32_SFLOAT;
			case Renderer::ShaderDataType::Float2:  return VK_FORMAT_R32G32_SFLOAT;
			case Renderer::ShaderDataType::Float3:  return VK_FORMAT_R32G32B32_SFLOAT;
			case Renderer::ShaderDataType::Float4:  return VK_FORMAT_R32G32B32A32_SFLOAT;
			case Renderer::ShaderDataType::Mat3:    return VK_FORMAT_R32G32B32_SFLOAT; 
			case Renderer::ShaderDataType::Mat4:    return VK_FORMAT_R32G32B32A32_SFLOAT;
			case Renderer::ShaderDataType::Int:     return VK_FORMAT_R32_SINT;
			case Renderer::ShaderDataType::Int2:    return VK_FORMAT_R32G32_SINT;
			case Renderer::ShaderDataType::Int3:    return VK_FORMAT_R32G32B32_SINT;
			case Renderer::ShaderDataType::Int4:    return VK_FORMAT_R32G32B32A32_SINT;
			case Renderer::ShaderDataType::Bool:    return VK_FORMAT_R8_UINT;
		}
		VT_CORE_ASSERT(false, "Unknown Shader Data Type");

		return VK_FORMAT_UNDEFINED; 
	}

	Application::Application()
	{
		VT_PROFILE_FUNCTION();
		s_Instance = this;

		m_WindowProps.m_Width = WIDTH;
		m_WindowProps.m_Height = HEIGHT;
		m_WindowProps.m_Title = TITLE;

		m_Context = std::make_unique<Renderer::Vulkan::VulkanContext>();
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

	void Application::PushOverlay(Layer* overlay)
	{
		VT_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);

		if (auto imgui = dynamic_cast<ImGuiLayer*>(overlay))
		{
			m_ImGuiLayer = imgui; 
		}
	}



	void Application::Init() 
	{

		m_Context->Init(); 
		m_Window->CreateSwapchain(); 
		Renderer::Renderer::Init();

		PushOverlay(new Velt::ImGuiLayer);
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

				ImGuiLayer::ProcessSDLEvent(&event);
				switch (event.type)
				{
				case SDL_EVENT_QUIT:
					{
						VT_PROFILE_SCOPE("WindowClose Event");
						WindowCloseEvent e; 
						OnEvent(e); 
						running = false;
						break;
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


			VT_PROFILE_SCOPE("Render Loop");
			Renderer::Renderer::BeginFrame();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			Renderer::Renderer::BeginScenePass();

			for (Layer* layer : m_LayerStack)
				layer->OnRender(m_Window->GetSwapchain().GetCurrentDrawCommandBuffer());
			
			Renderer::Renderer::EndScenePass();
			Renderer::Renderer::BeginGuiPass();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			// m_ImGuiLayer->Render(); 

			Renderer::Renderer::EndGuiPass();
			Renderer::Renderer::EndFrame();
			

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
