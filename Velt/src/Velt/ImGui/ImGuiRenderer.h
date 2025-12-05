#pragma once

#include "vtpch.h"
// ImGui headers - rely on the project's include paths to find ImGui. The
// CMake target that builds Velt should already add the vendor ImGui
// include directory to the compiler search path.
#include "imgui.h"
#include "Velt/Renderer/Core/Swapchain.h"
// Use the SDL3-specific ImGui headers located in vendor/ImGui/SDL3
// CMake adds ${CMAKE_CURRENT_SOURCE_DIR}/vendor/ImGui to the include
// directories, so we include the SDL3 path here.
#include "imgui_impl_vulkan.h"
#include "imgui_impl_sdl3.h"

struct VkDescriptorPool_T;
struct VkCommandBuffer_T;
struct VkRenderPass_T;
struct VkFramebuffer_T;

namespace Velt {
	class Velt_API ImGuiRenderer {
	public:
		ImGuiRenderer(RenderAPI::SwapChain* swapchain, RenderAPI::GPUDevice* device, SDL_Window* window);
		~ImGuiRenderer() = default;

		
		void Init();
		void Render(VkCommandBuffer& commandBuffer);
		void Shutdown();
		void Begin();
		void End();
		
	private:
		RenderAPI::SwapChain* m_Swapchain;
		RenderAPI::GPUDevice* m_Device;
		VkDescriptorPool_T* m_DescriptorPool = nullptr;
		SDL_Window* m_Window = nullptr;

		float m_Time = 0.0f;
	};
}