#pragma once
#include "Core/Core.h"
#include "Core/Timestep.h"
#include "Renderer/Texture.h"
#include "Panels/EditorViewportPanel.h"
#include "Panels/EditorSceneHierarchyPanel.h"
#include "Scene/Scene.h"
#include "Events/Event.h"

#include <ImGuizmo.h>
#include <entt/entt.hpp>

namespace Velt::Editor 
{
	class VELT_API EditorGuizmos 
	{
	public: 
		EditorGuizmos() = default;
		virtual ~EditorGuizmos() = default;

		virtual void Init(const Ref<Scene>& scene);
		virtual void Shutdown();

		static void BeginFrame() { ImGuizmo::BeginFrame(); }
		virtual void RenderGizmos(const Ref<EditorCamera>& camera);

		virtual void OnUpdate(Timestep ts, const Ref<ViewportPanel>& editorPanel, const Ref<SceneHierarchyPanel>& shPanel);
		virtual void OnEvent(Event& event);

		virtual void OnImGuiRender();
		virtual void OnImGuiRender2();
	private: 
		Ref<Texture2D> m_MousePickingRenderTarget;
		Ref<Scene> m_SceneContext;

		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
		Entity m_SelectedEntity;
	};
}
