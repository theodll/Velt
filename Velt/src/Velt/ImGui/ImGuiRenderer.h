#pragma once

#include "vtpch.h"
#include "imgui.h"
#include "Velt/Platform/Vulkan/VulkanSwapchain.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_sdl3.h"

struct VkDescriptorPool_T;
struct VkCommandBuffer_T;
struct VkRenderPass_T;
struct VkFramebuffer_T;

namespace Velt {
	class VELT_API ImGuiRenderer {
	public:
		ImGuiRenderer();
		~ImGuiRenderer() = default;

		
		void Init();
		void Render(VkCommandBuffer& commandBuffer);
		void Shutdown();
		void Begin();
		void End();
		
	private:
		VkDescriptorPool_T* m_DescriptorPool = nullptr;
		
		float m_Time = 0.0f;
	};
}