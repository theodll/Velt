#pragma once
/*

#include "vtpch.h"
#include "Velt/Layers/Layer.h"
#include "ImGuiRenderer.h"
#include "Widgets/DemoWidget.h"

#include "Velt/Platform/Vulkan/VulkanDevice.h"	
#include "Velt/Renderer/Renderer.h"

struct SDL_Window;
union SDL_Event;

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
		
		// OnRender: Called during the rendering phase after all scene geometry has been rendered.
		// This is where ImGui actually renders its prepared draw data to the command buffer.
		// By using the layer system's OnRender, ImGui respects the layer stack order and
		// renders on top of everything that was rendered before it in the stack.
		virtual void OnRender(void* commandBuffer) override;

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
*/