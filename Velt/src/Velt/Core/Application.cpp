#include "vtpch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Log.h"
#include <SDL3/SDL.h>
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Buffer.h"
#include "Events/ApplicationEvents.h" 
#include <cassert>
#include "Input.h"
#include "Events/EventHandler.h"
#include "Platform/SDL/SDLEventTranslator.h"

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

		m_Context = std::make_unique<Renderer::RHI::VulkanContext>();
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
			if (e.IsHandled)
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

		SDL::Input::Init();
	}

	void Application::Run()
	{
		VT_PROFILE_FUNCTION();

		bool running = true;
		while (running)
		{
			VT_PROFILE_SCOPE("Application::Run Loop");
			// TODO: Move to a Platform Independent Place

			static const double invFreq = 1.0 / (double)SDL_GetPerformanceFrequency();

			u64 counter = SDL_GetPerformanceCounter();
			double time = (double)counter * invFreq;

			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (SDL::Input::IsKeyDown(Scancode::VELT_SCANCODE_ESCAPE))
			{
				s_ShutdownRequested = true;
			}

			SDL_Event sdl;
			while (SDL_PollEvent(&sdl))
			{
				if (auto evt = SDL::TranslateSDLEvent(sdl)) {
					OnEvent(*evt);
				}
			}

			VT_PROFILE_SCOPE("Render Loop");
			Renderer::Renderer::BeginFrame();
			// Frame 
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(ts);

			Renderer::Renderer::BeginScenePass();
			// Scene Pass

			for (Layer* layer : m_LayerStack)
				layer->OnRender(m_Window->GetSwapchain().GetCurrentDrawCommandBuffer());
			
			Renderer::Renderer::EndScenePass();
			Renderer::Renderer::BeginGuiPass();
			// Gui Pass

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			RenderStatisticsWidget(ts);

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


	void Application::RenderStatisticsWidget(Timestep ts)
	{
		ImGui::Begin("Statistics");
		ImGui::Text("Velt Engine v0.0");
		float v = std::round(1 / ts.GetSeconds());
		static float s = v; s += (v - s) * (1.0f - std::exp(-ts.GetSeconds() / 0.12f)); // Smothes the fps display
		ImGui::Text("Frames per Second: %.0fFPS", s);
		ImGui::Dummy({ 500, 3 });
		ImGui::Text("Deltatime (s): %fs", ts.GetSeconds());
		ImGui::Text("Deltatime (ms): %.4gms", ts.GetMilliseconds());
		ImGui::Dummy({ 500, 3 });
		ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f", Renderer::SceneRenderer::GetCamera()->GetPosition().x, Renderer::SceneRenderer::GetCamera()->GetPosition().y, Renderer::SceneRenderer::GetCamera()->GetPosition().z);
		ImGui::End();
	}


	void Application::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		s_ShutdownRequested = true; 
	}
}
