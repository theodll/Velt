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
#include "Editor/FontLibrary/FontLibrary.h"
#include "Editor/EditorGuizmos.h"
#include "Core/Math.h"

namespace Velt {

	float ToLinear(float srgb) {
		return std::pow(srgb, 2.2f);
	}

	Ref<ImGuiRenderer> ImGuiLayer::m_Renderer = nullptr;

	VELT_API ImGuiContext* GetCurrentImGuiContext() 
	{
		return ImGui::GetCurrentContext();
	}

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

		const auto& device = RHI::VulkanContext::GetDevice();
		SDL_Window* window = nullptr;
	
		window = reinterpret_cast<SDL_Window*>(Application::Get()->GetWindow()->GetNativeHandle());
		
		m_Renderer = CreateRef<ImGuiRenderer>();
		
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
	
		Editor::FontLibrary::Get().Init();
		
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		// io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/segoe-ui/segoeuithis.ttf", 16.0f);

		ImGuiStyle& style = ImGui::GetStyle();

		ImVec4 panelBg =	 { ToLinear(38.0f / 255.0f), ToLinear(38.0f / 255.0f), ToLinear(38.0f / 255.0f), 1.0f };
		ImVec4 tabBar =		 { ToLinear(21.0f / 255.0f), ToLinear(21.0f / 255.0f), ToLinear(21.0f / 255.0f), 1.0f };
		ImVec4 hoveredTab = { ToLinear(49.0f / 255.0f), ToLinear(45.0f / 255.0f), ToLinear(34.0f / 255.0f), 1.0f };
		ImVec4 tab =		 { ToLinear(56.0f / 255.0f), ToLinear(52.0f / 255.0f), ToLinear(41.0f / 255.0f), 1.0f };

		ImVec4  accentColor = { ToLinear(212.0f / 255.0f), ToLinear(184.0f / 255.0f), ToLinear(138.0f / 255.0f), 1.0f };
		ImVec4	hoveredAccentColor = { ToLinear(47.0f / 255.0f), ToLinear(47.0f / 255.0f), ToLinear(47.0f / 255.0f), 1.0f };

		// ApplyEditorTheme(style);
		style.Colors[ImGuiCol_WindowBg] =				panelBg;
		style.Colors[ImGuiCol_TitleBg] =				tabBar;
		style.Colors[ImGuiCol_TitleBgActive] =			tabBar;
		style.Colors[ImGuiCol_MenuBarBg] =				tabBar;

		style.Colors[ImGuiCol_TabHovered] =				hoveredTab;
		style.Colors[ImGuiCol_Tab] =					tab;
		style.Colors[ImGuiCol_TabSelected] =			tab;
		style.Colors[ImGuiCol_TabSelectedOverline] =	tab;
		style.Colors[ImGuiCol_TabDimmed] =				tabBar;
		style.Colors[ImGuiCol_TabDimmedSelected] =		tabBar;

		//style.Colors[ImGuiCol_HeaderActive] =			accentColor;
		//style.Colors[ImGuiCol_Header] =					accentColor;
		style.Colors[ImGuiCol_HeaderHovered] =			hoveredAccentColor;


		m_Renderer->Init();

		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		
		VT_CORE_INFO("ImGuiLayer attached");
	}

	void ImGuiLayer::OnDetach()
	{
		VT_PROFILE_FUNCTION();
		if (m_Renderer)
			m_Renderer->Shutdown();
		ImGui::DestroyContext();
		VT_CORE_INFO("ImGuiLayer detached");
	}

	void ImGuiLayer::OnEvent(Event& rEvent)
	{
		VT_PROFILE_FUNCTION();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

	}

	void ImGuiLayer::ProcessSDLEvent(const SDL_Event* pEvent)
	{
		VT_PROFILE_FUNCTION();
		// Forward raw SDL event to ImGui SDL3 backend
		ImGui_ImplSDL3_ProcessEvent(reinterpret_cast<SDL_Event*>(const_cast<SDL_Event*>(pEvent)));
	}

	void ImGuiLayer::OnUpdate(Timestep ts)
	{
		VT_PROFILE_FUNCTION();
	}

	void ImGuiLayer::OnImGuiRender()
	{
		VT_PROFILE_FUNCTION();

		Editor::EditorGuizmos::BeginFrame();
		SetupDockspace();
	}

	void ImGuiLayer::OnImGuiRender2()
	{
		VT_PROFILE_FUNCTION();
		

		
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
		
	}
	void ImGuiLayer::OnRender(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();
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

	void ImGuiLayer::Render()
	{
		VT_PROFILE_FUNCTION();

		auto&& cmd = Velt::Application::Get()->GetWindow()->GetSwapchain()->GetCurrentDrawCommandBuffer();
		m_Renderer->RenderDrawData(cmd);
	}
}

