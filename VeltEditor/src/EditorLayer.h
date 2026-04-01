#pragma once
#include <Velt.h>
#include <Velt/Editor/Panels/EditorViewportPanel.h>

namespace Velt::Editor {

	class EditorLayer : public Velt::Layer
	{
	public:
		EditorLayer();

		void Init() override;
		void Shutdown() override;

		void OnAttach() override;
		void OnDetach() override; 
		void OnUpdate(Velt::Timestep ts) override;
		void OnEvent(Velt::Event& event) override;
		void OnBeforeFrameBegin() override;

		void OnRender(VkCommandBuffer commandBuffer) override;
		void OnDefferedRender(VkCommandBuffer commandBuffer) override;

		void OnImGuiRender2() override;

	private: 
		Ref<Scene> m_ActiveScene;

		Ref<SceneRenderer> m_SceneRenderer;
		Ref<DefferedRenderer> m_DefferedRenderer;

		Ref<EditorCamera> m_EditorCamera;

		Ref<EditorViewportPanel> m_ViewportPanel;
	};
}