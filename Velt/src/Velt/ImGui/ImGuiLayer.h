#pragma once

#include "vtpch.h"
#include "Velt/Core/Layer.h"
#include "ImGuiRenderer.h"

#include "Velt/Platform/Vulkan/VulkanDevice.h"	
#include "Velt/Renderer/Renderer.h"

namespace Velt {

	VELT_API ImGuiContext* GetCurrentImGuiContext();

	class VELT_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		ImGuiLayer(const ImGuiLayer&) = delete;
		ImGuiLayer& operator=(const ImGuiLayer&) = delete;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& rEvent) override;
		virtual void OnUpdate(Timestep ts) override;
		
		virtual void OnRender(VkCommandBuffer commandBuffer) override;
		virtual void OnImGuiRender() override;
		virtual void OnImGuiRender2() override;

		static void ProcessSDLEvent(const SDL_Event* pEvent);

		static void Begin();
		static void End();
		static void Render();

		void SetupDockspace();
		void RenderSceneViewport();
		
		static ImGuiRenderer* GetRenderer() { return m_Renderer.get(); } 
		
	private: 
		inline void ApplyEditorTheme(ImGuiStyle& style);
		static Ref<ImGuiRenderer> m_Renderer;

		Scope<RHI::VulkanDevice> m_Device;
		
	}; 
}
