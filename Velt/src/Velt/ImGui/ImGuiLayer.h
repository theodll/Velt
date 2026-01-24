#pragma once

#include "vtpch.h"
#include "Velt/Layers/Layer.h"
#include "ImGuiRenderer.h"
#include "Widgets/DemoWidget.h"
#include "SceneViewport.h"

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
		virtual void OnImGuiRender() override;

		static void ProcessSDLEvent(const SDL_Event* event);

		static void Begin();
		static void End();

		void SetupDockspace();
		void RenderSceneViewport();
		
		static ImGuiRenderer* GetRenderer() { return m_Renderer.get(); } // <- hier hakt es
		static SceneViewport* GetViewport() { return m_SceneViewport.get(); }

	private: 
		inline void ApplyEditorTheme(ImGuiStyle& style);
		static Ref<ImGuiRenderer> m_Renderer;

		Scope<Renderer::Vulkan::VulkanDevice> m_Device;
		static Scope<SceneViewport> m_SceneViewport;


		DemoWidget m_DemoWidget;

	}; 
}
