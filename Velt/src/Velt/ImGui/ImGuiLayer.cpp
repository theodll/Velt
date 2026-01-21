#include "vtpch.h"
#include "ImGuiLayer.h"
#include <SDL3/SDL.h>
#include "ImGuiRenderer.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "Velt/Core/Log.h"
#include "Velt/Renderer/Renderer.h"
#include "Velt/Platform/Vulkan/VulkanContext.h"
#include "Velt/Core/Application.h"

namespace Velt {
	
	
	ImGuiLayer::ImGuiLayer()
	{
		VT_PROFILE_FUNCTION();
	}

	ImGuiLayer::~ImGuiLayer()
	{
		VT_PROFILE_FUNCTION();
		if (m_Renderer)
			m_Renderer->Shutdown();
	}

	void ImGuiLayer::OnAttach()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Attach ImGuiLayer");

		auto& device = Renderer::Vulkan::VulkanContext::GetDevice();
		SDL_Window* window = nullptr;
	
		window = reinterpret_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeHandle());
		
		m_Renderer = CreateRef<ImGuiRenderer>();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Initialize platform/renderer backends
		m_Renderer->Init();

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		
		// Create scene viewport
		m_SceneViewport = CreateScope<SceneViewport>(m_ViewportWidth, m_ViewportHeight);
		
		VT_CORE_INFO("ImGuiLayer attached");
	}

	void ImGuiLayer::OnDetach()
	{
		VT_PROFILE_FUNCTION();
		m_SceneViewport.reset();
		if (m_Renderer)
			m_Renderer->Shutdown();
		ImGui::DestroyContext();
		VT_CORE_INFO("ImGuiLayer detached");
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		VT_PROFILE_FUNCTION();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
	}

	void ImGuiLayer::ProcessSDLEvent(const SDL_Event* event)
	{
		VT_PROFILE_FUNCTION();
		ImGui_ImplSDL3_ProcessEvent(reinterpret_cast<SDL_Event*>(const_cast<SDL_Event*>(event)));
	}

	void ImGuiLayer::OnUpdate()
	{
		VT_PROFILE_FUNCTION();
		// Begin ImGui frame
		m_Renderer->Begin();

		// Setup dockspace
		SetupDockspace();

		// Render scene viewport
		RenderSceneViewport();

		// Render demo widget
		m_DemoWidget.OnRender();

		// End ImGui frame (prepares draw data)
		m_Renderer->End();
	}

	void ImGuiLayer::SetupDockspace()
	{
		VT_PROFILE_FUNCTION();
		
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		// Menu bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))
				{
					Application::s_ShutdownRequested = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

	void ImGuiLayer::RenderSceneViewport()
	{
		VT_PROFILE_FUNCTION();
		
		// Fixed size for the scene viewport
		const ImVec2 viewportSize(static_cast<float>(m_ViewportWidth), static_cast<float>(m_ViewportHeight));
		
		ImGui::SetNextWindowSize(viewportSize, ImGuiCond_Always);
		ImGui::Begin("Scene Viewport", nullptr, 
			ImGuiWindowFlags_NoResize | 
			ImGuiWindowFlags_NoScrollbar | 
			ImGuiWindowFlags_NoScrollWithMouse);
		
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		
		// Display the rendered scene texture
		if (m_SceneViewport && m_SceneViewport->GetDescriptorSet() != VK_NULL_HANDLE)
		{
			ImGui::Image((ImTextureID)m_SceneViewport->GetDescriptorSet(), contentRegion);
		}
		else
		{
			ImGui::Text("Scene viewport not initialized");
		}
		
		ImGui::End();
	}

	void ImGuiLayer::OnRender(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();
		if (commandBuffer != nullptr)
		{
			VT_PROFILE_SCOPE("ImGuiLayer::OnRender Render");
			m_Renderer->Render(commandBuffer);
		}
	}

	void ImGuiLayer::Begin()
	{
		VT_PROFILE_FUNCTION();
		m_Renderer->Begin();
	}

	void ImGuiLayer::End()
	{
		VT_PROFILE_FUNCTION();
		m_Renderer->End();
	}
}

