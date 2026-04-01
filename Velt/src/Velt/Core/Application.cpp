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

	VkFormat ShaderDataTypeToVulkanBaseType(ShaderDataType type) {
		switch(type) {
			case ShaderDataType::Float:   return VK_FORMAT_R32_SFLOAT;
			case ShaderDataType::Float2:  return VK_FORMAT_R32G32_SFLOAT;
			case ShaderDataType::Float3:  return VK_FORMAT_R32G32B32_SFLOAT;
			case ShaderDataType::Float4:  return VK_FORMAT_R32G32B32A32_SFLOAT;
			case ShaderDataType::Mat3:    return VK_FORMAT_R32G32B32_SFLOAT; 
			case ShaderDataType::Mat4:    return VK_FORMAT_R32G32B32A32_SFLOAT;
			case ShaderDataType::Int:     return VK_FORMAT_R32_SINT;
			case ShaderDataType::Int2:    return VK_FORMAT_R32G32_SINT;
			case ShaderDataType::Int3:    return VK_FORMAT_R32G32B32_SINT;
			case ShaderDataType::Int4:    return VK_FORMAT_R32G32B32A32_SINT;
			case ShaderDataType::Bool:    return VK_FORMAT_R8_UINT;
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

		m_Context = std::make_unique<RHI::VulkanContext>();
		m_Window = std::unique_ptr<Window>(Window::Create(m_WindowProps));
		m_Context->Init();

		VT_CORE_TRACE("Tracy Version: v0.13.1");
	}


	void Application::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		m_LayerStack.PopAllLayers();
		for (auto* layer : m_LayerStack)
			delete layer;

		Renderer::QueueShutdown();
		m_Window->Shutdown();
		m_Context->Shutdown();
	}

	Application::~Application()
	{
		VT_PROFILE_FUNCTION();
	}

	void Application::OnEvent(Event& e)
	{
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
		VT_PROFILE_FUNCTION();
		m_Window->CreateSwapchain(); 
		Renderer::Init();

		PushOverlay(new Velt::ImGuiLayer);

		Input::Init();

		for (Layer* layer : m_LayerStack)
			layer->Init();
	}

	void Application::Run()
	{
		bool running = true;

		while (running)
		{
			// TODO: Move to a Platform Independent Place

			static const double invFreq = 1.0 / (double)SDL_GetPerformanceFrequency();

			u64 counter = SDL_GetPerformanceCounter();
			double time = (double)counter * invFreq;

			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (Input::IsKeyDown(Scancode::VELT_SCANCODE_ESCAPE))
			{
				s_ShutdownRequested = true;
			}

			SDL_Event sdl;
			while (SDL_PollEvent(&sdl))
			{
				if (auto evt = SDL::TranslateSDLEvent(&sdl)) {
					OnEvent(*evt);
				}
			}

			{
				i32 pixelW = 0, pixelH = 0;
				auto* sdlWindow = static_cast<SDL_Window*>(m_Window->GetNativeHandle());
				SDL_GetWindowSizeInPixels(sdlWindow, &pixelW, &pixelH);

				if (pixelW == 0 || pixelH == 0)
				{
					SDL_Delay(16);
					continue;
				}

				const auto& swapchain = m_Window->GetSwapchain();
				if ((u32)pixelW != swapchain->GetWidth() || (u32)pixelH != swapchain->GetHeight())
				{
					SwapchainExtent extent{ (u32)pixelW, (u32)pixelH };
					swapchain->OnResize(&extent);
				}
			}

			VT_PROFILE_SCOPE("Render Loop");

			auto cmd = m_Window->GetSwapchain()->GetCurrentDrawCommandBuffer();

			for (Layer* layer : m_LayerStack)
				layer->OnBeforeFrameBegin();

			Renderer::BeginFrame();
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(ts);

			Renderer::BeginScenePass();
			for (Layer* layer : m_LayerStack)
				layer->OnRender(cmd);
			Renderer::EndScenePass();

			Renderer::BeginDefferedPass();
			for (Layer* layer : m_LayerStack)
				layer->OnDefferedRender(cmd);
			Renderer::EndDefferedPass();

			Renderer::BeginGuiPass();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();

			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender2();

			RenderStatisticsWidget(ts);
			Renderer::EndGuiPass();

			Renderer::EndFrame();
			

			if (s_ShutdownRequested)
			{
				Shutdown();
				Velt::Log::Flush();
				break;
			}
		}
	}


	static const char* RenderTargetToString(RenderTarget rt)
	{
		switch (rt)
		{
		case VT_RENDER_TARGET_ALBEDO_AO:   return "Albedo + Ambient Occlusion";
		case VT_RENDER_TARGET_NORMAL_ROUGH:   return "Normal + Roughness";
		case VT_RENDER_TARGET_METAL_EMIT:    return "Metal + Emit";
		case VT_RENDER_TARGET_COMPOSITE: return "Composite";
		default: return "Unknown";
		}
	}

	void Application::RenderStatisticsWidget(Timestep ts)
	{
		/*
		ImGui::Begin("Statistics");
		ImGui::Text("Velt Engine v0.0");
		ImGui::Separator();
		float v = std::round(1 / ts.GetSeconds());
		static float s = v; s += (v - s) * (1.0f - std::exp(-ts.GetSeconds() / 0.12f)); // Smothes the fps display
		ImGui::Text("Frames per Second: %.0fFPS", s);
		ImGui::Dummy({ 500, 3 });
		ImGui::Text("Deltatime (s): %fs", ts.GetSeconds());
		ImGui::Text("Deltatime (ms): %.4gms", ts.GetMilliseconds());
		ImGui::Dummy({ 500, 3 });
		ImGui::Separator();
		ImGui::Text("Draw Calls: %i", Renderer::GetDrawCallCount());
		
		static RenderTarget currentRT = (RenderTarget)Application::Get()->SelectedRenderTarget;

		if (ImGui::BeginCombo("Render Target", RenderTargetToString(currentRT)))
		{
			for (int i = 0; i < 5; i++)
			{
				RenderTarget rt = (RenderTarget)i;
				bool selected = (currentRT == rt);

				if (ImGui::Selectable(RenderTargetToString(rt), selected))
				{
					currentRT = rt;
					ImGuiLayer::m_PendingRenderTarget = rt;
					ImGuiLayer::m_RenderTargetChangePending = true;
				}

				if (selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
//		ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f", SceneRenderer::GetCamera()->GetPosition().x, SceneRenderer::GetCamera()->GetPosition().y, SceneRenderer::GetCamera()->GetPosition().z);
		ImGui::End();

		Velt::Application::UpdateTitle(Velt::Application::Get()->TITLE + " - " + std::to_string((int)s), false);
		*/
	}
}
