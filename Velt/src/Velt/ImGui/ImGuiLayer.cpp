
#include "vtpch.h"
#include "ImGuiLayer.h"
#include <SDL3/SDL.h>
#include "ImGuiRenderer.h"
// ImGui core and backend for SDL3 are provided under vendor/ImGui/SDL3.
// Include the SDL3-specific headers (CMake adds vendor/ImGui to include paths).
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "Velt/Core/Log.h"
#include "Velt/Renderer/Renderer.h"
#include "Velt/Platform/Vulkan/VulkanContext.h"
#include "Velt/Core/Application.h"


namespace Velt {

	Scope<SceneViewport> ImGuiLayer::m_SceneViewport = nullptr;
	Ref<ImGuiRenderer> ImGuiLayer::m_Renderer = nullptr;

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


		auto& device = Renderer::RHI::VulkanContext::GetDevice();
		SDL_Window* window = nullptr;
	
		window = reinterpret_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeHandle());
		

		m_Renderer = CreateRef<ImGuiRenderer>();
		m_SceneViewport = CreateScope<SceneViewport>();
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		// ApplyEditorTheme(style);
	

		m_Renderer->Init();
		m_SceneViewport->Init(480, 480);


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

	void ImGuiLayer::OnEvent(Event& event)
	{
		VT_PROFILE_FUNCTION();
		// Handle ImGui input events forwarded from Application via engine Events.
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// We rely on the SDL backend to process raw SDL events; engine Events may be
		// extended later to translate into ImGui IO if needed.
	}

	void ImGuiLayer::ProcessSDLEvent(const SDL_Event* event)
	{
		VT_PROFILE_FUNCTION();
		// Forward raw SDL event to ImGui SDL3 backend
		ImGui_ImplSDL3_ProcessEvent(reinterpret_cast<SDL_Event*>(const_cast<SDL_Event*>(event)));
	}

	void ImGuiLayer::OnUpdate()
	{
		VT_PROFILE_FUNCTION();
	}

	void ImGuiLayer::ApplyEditorTheme(ImGuiStyle& style)
	{
		// --- Spacing / Rounding (macht den "modern editor" Look aus) ---
		style.WindowPadding = ImVec2(0.0f, 0.0f);
		style.FramePadding = ImVec2(0.0f, 0.0f);
		style.CellPadding = ImVec2(0.0f, 0.0f);
		style.ItemSpacing = ImVec2(8.0f, 6.0f);
		style.ItemInnerSpacing = ImVec2(0.0f, 0.0f);
		style.IndentSpacing = 18.0f;
		style.ScrollbarSize = 14.0f;
		style.GrabMinSize = 10.0f;

		style.WindowRounding = 10.0f;
		style.ChildRounding = 10.0f;
		style.FrameRounding = 8.0f;
		style.PopupRounding = 10.0f;
		style.ScrollbarRounding = 12.0f;
		style.GrabRounding = 10.0f;
		style.TabRounding = 0.0f;

		style.WindowBorderSize = 1.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupBorderSize = 1.0f;
		style.FrameBorderSize = 0.0f;
		style.TabBorderSize = 0.0f;

		style.WindowTitleAlign = ImVec2(0.50f, 0.50f);
		style.ButtonTextAlign = ImVec2(0.50f, 0.50f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.50f);

		style.SeparatorTextBorderSize = 0.0f;
		style.SeparatorTextAlign = ImVec2(0.0f, 0.5f);

		style.AntiAliasedLines = true;
		style.AntiAliasedFill = true;

		ImVec4* c = style.Colors;

		const ImVec4 bg0 = ImVec4(0.07f, 0.07f, 0.08f, 1.00f);
		const ImVec4 bg1 = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
		const ImVec4 bg2 = ImVec4(0.13f, 0.13f, 0.16f, 1.00f);
		const ImVec4 fg0 = ImVec4(0.90f, 0.90f, 0.92f, 1.00f);
		const ImVec4 fg1 = ImVec4(0.70f, 0.70f, 0.74f, 1.00f);

		const ImVec4 accent = ImVec4(0.20f, 0.45f, 0.95f, 1.00f);
		const ImVec4 accentHover = ImVec4(0.25f, 0.52f, 0.98f, 1.00f);
		const ImVec4 accentActive = ImVec4(0.18f, 0.40f, 0.86f, 1.00f);

		c[ImGuiCol_Text] = fg0;
		c[ImGuiCol_TextDisabled] = ImVec4(fg1.x, fg1.y, fg1.z, 1.0f);

		c[ImGuiCol_WindowBg] = bg0;
		c[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0);
		c[ImGuiCol_PopupBg] = bg1;

		c[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.24f, 1.00f);
		c[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

		c[ImGuiCol_FrameBg] = bg1;
		c[ImGuiCol_FrameBgHovered] = bg2;
		c[ImGuiCol_FrameBgActive] = ImVec4(bg2.x, bg2.y, bg2.z, 1.00f);

		c[ImGuiCol_TitleBg] = bg0;
		c[ImGuiCol_TitleBgActive] = bg0;
		c[ImGuiCol_TitleBgCollapsed] = bg0;

		c[ImGuiCol_MenuBarBg] = bg1;

		c[ImGuiCol_ScrollbarBg] = ImVec4(bg0.x, bg0.y, bg0.z, 0.60f);
		c[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
		c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.30f, 0.36f, 1.00f);
		c[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.34f, 0.34f, 0.40f, 1.00f);

		c[ImGuiCol_CheckMark] = accent;

		c[ImGuiCol_SliderGrab] = accent;
		c[ImGuiCol_SliderGrabActive] = accentActive;

		c[ImGuiCol_Button] = ImVec4(bg1.x, bg1.y, bg1.z, 1.00f);
		c[ImGuiCol_ButtonHovered] = ImVec4(bg2.x, bg2.y, bg2.z, 1.00f);
		c[ImGuiCol_ButtonActive] = ImVec4(bg2.x, bg2.y, bg2.z, 1.00f);

		c[ImGuiCol_Header] = ImVec4(bg1.x, bg1.y, bg1.z, 1.00f);
		c[ImGuiCol_HeaderHovered] = ImVec4(bg2.x, bg2.y, bg2.z, 1.00f);
		c[ImGuiCol_HeaderActive] = ImVec4(bg2.x, bg2.y, bg2.z, 1.00f);

		c[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.24f, 1.00f);
		c[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.30f, 0.36f, 1.00f);
		c[ImGuiCol_SeparatorActive] = ImVec4(0.35f, 0.35f, 0.42f, 1.00f);

		c[ImGuiCol_Tab] = bg1;
		c[ImGuiCol_TabHovered] = bg2;
		c[ImGuiCol_TabActive] = bg2;
		c[ImGuiCol_TabUnfocused] = bg1;
		c[ImGuiCol_TabUnfocusedActive] = bg2;

		c[ImGuiCol_ResizeGrip] = ImVec4(0, 0, 0, 0);
		c[ImGuiCol_ResizeGripHovered] = ImVec4(0, 0, 0, 0);
		c[ImGuiCol_ResizeGripActive] = ImVec4(0, 0, 0, 0);

		c[ImGuiCol_PlotLines] = fg1;
		c[ImGuiCol_PlotLinesHovered] = fg0;
		c[ImGuiCol_PlotHistogram] = accent;
		c[ImGuiCol_PlotHistogramHovered] = accentHover;

		c[ImGuiCol_TextSelectedBg] = ImVec4(accent.x, accent.y, accent.z, 0.35f);
		c[ImGuiCol_DragDropTarget] = ImVec4(accent.x, accent.y, accent.z, 0.90f);

		c[ImGuiCol_NavCursor] = ImVec4(accent.x, accent.y, accent.z, 1.00f);
		c[ImGuiCol_NavWindowingHighlight] = ImVec4(accent.x, accent.y, accent.z, 0.80f);
	}

	void ImGuiLayer::OnImGuiRender()
	{
		VT_PROFILE_FUNCTION();

		SetupDockspace();
		RenderSceneViewport();
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
		const ImVec2 viewportSize(static_cast<float>(m_SceneViewport->GetWidth()), static_cast<float>(m_SceneViewport->GetHeight()));

		// ImGui::SetNextWindowSize(viewportSize, ImGuiCond_Always);
		ImGui::Begin("Scene Viewport", nullptr,
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
	//	if (commandBuffer != nullptr)
	//	{
	//		VT_PROFILE_SCOPE("ImGuiLayer::OnRender Render");
	//		m_Renderer->Render(commandBuffer);
	//	}
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

