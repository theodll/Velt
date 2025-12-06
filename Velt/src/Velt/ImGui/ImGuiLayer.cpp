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

namespace Velt
{

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
		// Use static accessors from Renderer to get device and swapchain
		auto *swapchain = Velt::RenderAPI::Renderer::getSwapChain();
		auto *device = Velt::RenderAPI::Renderer::getDevice();
		SDL_Window *window = nullptr;
		if (device)
		{
			window = reinterpret_cast<SDL_Window *>(device->getWindow().GetNativeHandle());
		}

		m_Renderer = CreateRef<ImGuiRenderer>(
			swapchain,
			device,
			window);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
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

	void ImGuiLayer::OnEvent(Event &event)
	{
		VT_PROFILE_FUNCTION();
		// Handle ImGui input events forwarded from Application via engine Events.
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		// We rely on the SDL backend to process raw SDL events; engine Events may be
		// extended later to translate into ImGui IO if needed.
	}

	void ImGuiLayer::ProcessSDLEvent(const SDL_Event *event)
	{
		VT_PROFILE_FUNCTION();
		// Forward raw SDL event to ImGui SDL3 backend
		ImGui_ImplSDL3_ProcessEvent(reinterpret_cast<SDL_Event *>(const_cast<SDL_Event *>(event)));
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

	void ImGuiLayer::OnRender(void *commandBuffer)
	{
		VT_PROFILE_FUNCTION();
		// This method is called during the rendering phase after all scene geometry has been rendered.
		// It takes the VkCommandBuffer that was passed through the layer system and renders the ImGui
		// draw data that was prepared in OnUpdate(). This ensures ImGui renders on top of everything
		// in the correct layer order.

		if (commandBuffer != nullptr)
		{
			VT_PROFILE_SCOPE("ImGuiLayer::OnRender Render");
			VkCommandBuffer cmd = reinterpret_cast<VkCommandBuffer>(commandBuffer);
			m_Renderer->Render(cmd);
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
