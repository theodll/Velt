#pragma once
#include <Velt.h>
#include <Velt/Editor/Panels/EditorViewportPanel.h>
#include <Velt/Editor/EditorGuizmos.h>

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

		void OnImGuiRender() override;
		void OnImGuiRender2() override;

	private: 
		Ref<Scene> m_ActiveScene;

		Ref<SceneRenderer> m_SceneRenderer;
		Ref<DefferedRenderer> m_DefferedRenderer;

		Timestep m_Timestep;

		Ref<EditorCamera> m_EditorCamera;
		Ref<EditorGuizmos> m_EditorGuizmos;
		Ref<EditorViewportPanel> m_ViewportPanel;
	};
}