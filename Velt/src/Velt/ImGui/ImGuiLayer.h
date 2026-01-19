#pragma once

#include "vtpch.h"
#include "Velt/Layers/Layer.h"
#include "ImGuiRenderer.h"
#include "Widgets/DemoWidget.h"

#include "Velt/Platform/Vulkan/VulkanDevice.h"	
#include "Velt/Renderer/Renderer.h"

namespace Velt {
	class VELT_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		ImGuiLayer(const ImGuiLayer&) = delete;
		ImGuiLayer& operator=(const ImGuiLayer&) = delete;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate() override;
		
		virtual void OnRender(VkCommandBuffer commandBuffer) override;

		// Process a raw SDL event through ImGui's SDL backend (implemented in .cpp)
		static void ProcessSDLEvent(const SDL_Event* event);

		void Begin();
		void End();
		
		// Allow renderer to record ImGui commands
		ImGuiRenderer& GetRenderer() { return *m_Renderer; }

	private:
		Ref<ImGuiRenderer> m_Renderer;

		Scope<Renderer::Vulkan::VulkanDevice> m_Device;

		DemoWidget m_DemoWidget;

	}; 
}
