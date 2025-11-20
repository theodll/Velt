#include "lvpch.h"
#include "ImGuiLayer.h"
#include <SDL3/SDL.h>
#include "ImGuiRenderer.h"
// ImGui core and backend for SDL3 are provided under vendor/ImGui/SDL3.
// Include the SDL3-specific headers (CMake adds vendor/ImGui to include paths).
#include "SDL3/imgui.h"
#include "SDL3/imgui_impl_sdl3.h"
#include "Lavendel/Log.h"
#include "Lavendel/Renderer/Renderer.h"
#include "Lavendel/Log.h"

namespace Lavendel {
	
	
	ImGuiLayer::ImGuiLayer()
	{
		LV_PROFILE_FUNCTION();
	}

	ImGuiLayer::~ImGuiLayer()
	{
		LV_PROFILE_FUNCTION();
		if (m_Renderer)
			m_Renderer->Shutdown();
	}

	void ImGuiLayer::OnAttach()
	{
		LV_PROFILE_FUNCTION();
		// Use static accessors from Renderer to get device and swapchain
		auto swapchain = Lavendel::RenderAPI::Renderer::getSwapChain();
		auto device = Lavendel::RenderAPI::Renderer::getDevice();
		SDL_Window* window = nullptr;
		if (device)
		{
			window = reinterpret_cast<SDL_Window*>(device->getWindow().GetNativeHandle());
		}

		m_Renderer = std::make_shared<ImGuiRenderer>(
			swapchain,
			device,
			window
		);

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
		
		LV_CORE_INFO("ImGuiLayer attached");
	}

	void ImGuiLayer::OnDetach()
	{
		LV_PROFILE_FUNCTION();
		if (m_Renderer)
			m_Renderer->Shutdown();
		ImGui::DestroyContext();
		LV_CORE_INFO("ImGuiLayer detached");
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		LV_PROFILE_FUNCTION();
		// Handle ImGui input events forwarded from Application via engine Events.
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// We rely on the SDL backend to process raw SDL events; engine Events may be
		// extended later to translate into ImGui IO if needed.
	}

	void ImGuiLayer::ProcessSDLEvent(const SDL_Event* event)
	{
		LV_PROFILE_FUNCTION();
		// Forward raw SDL event to ImGui SDL3 backend
		ImGui_ImplSDL3_ProcessEvent(reinterpret_cast<SDL_Event*>(const_cast<SDL_Event*>(event)));
	}

	void ImGuiLayer::OnUpdate()
	{
		LV_PROFILE_FUNCTION();
		// Begin ImGui frame
		m_Renderer->Begin();

		// Render demo widget
		m_DemoWidget.OnRender();

		// End ImGui frame (prepares draw data)
		m_Renderer->End();
	}

	void ImGuiLayer::OnRender(void* commandBuffer)
	{
		LV_PROFILE_FUNCTION();
		// This method is called during the rendering phase after all scene geometry has been rendered.
		// It takes the VkCommandBuffer that was passed through the layer system and renders the ImGui
		// draw data that was prepared in OnUpdate(). This ensures ImGui renders on top of everything
		// in the correct layer order.
		
		if (commandBuffer != nullptr)
		{
			LV_PROFILE_SCOPE("ImGuiLayer::OnRender Render");
			VkCommandBuffer cmd = reinterpret_cast<VkCommandBuffer>(commandBuffer);
			m_Renderer->Render(cmd);
		}
	}

	void ImGuiLayer::Begin()
	{
		LV_PROFILE_FUNCTION();
		m_Renderer->Begin();
	}

	void ImGuiLayer::End()
	{
		LV_PROFILE_FUNCTION();
		m_Renderer->End();
	}
}
