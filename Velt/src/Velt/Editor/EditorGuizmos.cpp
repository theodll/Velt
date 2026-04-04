#include "vtpch.h"
#include "EditorGuizmos.h"
#include "Panels/EditorViewportPanel.h"
#include "Renderer/Renderer.h"
#include "Core/Input.h"



namespace Velt::Editor 
{
	void EditorGuizmos::Init(const Ref<Scene>& scene)
	{
		VT_PROFILE_FUNCTION();
		m_MousePickingRenderTarget = Renderer::GetRenderTarget(VT_RENDER_TARGET_MOUSE_PICKING);
		m_SceneContext = scene;
	}

	void EditorGuizmos::Shutdown()
	{
		VT_PROFILE_FUNCTION();
	}

	void EditorGuizmos::OnImGuiRender2()
	{
		VT_PROFILE_FUNCTION();
	}

	void EditorGuizmos::OnImGuiRender()
	{
		VT_PROFILE_FUNCTION();
	}

	void EditorGuizmos::OnUpdate(Timestep ts, const Ref<ViewportPanel>& editorPanel, const Ref<SceneHierarchyPanel>& shPanel)
	{
		VT_PROFILE_FUNCTION();
		if (Input::IsMouseKeyPressed(VELT_MOUSE1))
		{
			auto&& image = Renderer::GetRenderTarget(VT_RENDER_TARGET_MOUSE_PICKING);
			if (editorPanel->m_IsHovered)
			{


				u32 id = image->ReadPixel((u32)editorPanel->m_LocalMousePos.x, (u32)editorPanel->m_LocalMousePos.y);
				VT_CORE_INFO("{}", id);

				// 0xFFFFFFFF is the standard draw call value.
				// 0x3F800000 is the clear color represented by float 1.0f
				if (id == 0xFFFFFFFF || id == 0x3F800000 && !m_SelectedEntity)
					m_SelectedEntity = { (entt::entity)id, m_SceneContext.get() };  // entt::null is also equal to 0xFFFFFFFF so only needed for 0x3F800000
				if (id == 0xFFFFFFFF || id == 0x3F800000 && m_SelectedEntity) {}
				else
					m_SelectedEntity = { (entt::entity)id, m_SceneContext.get() };

				shPanel->SetSelected(m_SelectedEntity);

			}

		}
	}
	void EditorGuizmos::OnEvent(Event& event)
	{
		VT_PROFILE_FUNCTION();
		

	}

}