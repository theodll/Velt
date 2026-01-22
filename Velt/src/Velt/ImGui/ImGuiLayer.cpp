
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
		// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Commented out - may not be available in all versions
		
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Initialize platform/renderer backends
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
		// Begin ImGui frame
		m_Renderer->Begin();

		// Render demo widget
		m_DemoWidget.OnRender();

		// End ImGui frame (prepares draw data)
		m_Renderer->End();
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

