#include "vtpch.h"
#include "EditorGuizmos.h"
#include "Panels/EditorViewportPanel.h"
#include "Renderer/Renderer.h"
#include "Core/Input.h"

#include <glm/gtc/type_ptr.hpp>


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
		if (m_SelectedEntity != shPanel->GetSelected())
			m_SelectedEntity = shPanel->GetSelected();

		if (Input::IsMouseKeyPressed(VELT_MOUSE1) && !(ImGuizmo::IsUsingAny() || ImGuizmo::IsUsing() || ImGuizmo::IsOver()))
		{
			m_SceneHierarchyPanel = shPanel;
			auto&& image = Renderer::GetRenderTarget(VT_RENDER_TARGET_MOUSE_PICKING);
			if (editorPanel->m_IsHovered)
			{
				u32 id = image->ReadPixel((u32)editorPanel->m_LocalMousePos.x, (u32)editorPanel->m_LocalMousePos.y);

				// Note [Someday in 2026, Theo]
				// 0xFFFFFFFF is the standard draw call value.
				// 0x3F800000 is the clear color represented by float 1.0f
				if (id == 0xFFFFFFFF || id == 0x3F800000 && !m_SelectedEntity)
					m_SelectedEntity = { (entt::entity)id, m_SceneContext.get() };  // entt::null is equal to 0xFFFFFFFF / std::numeric_limits<uint32_t> so only needed for 0x3F800000
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

	void EditorGuizmos::RenderGizmos(const Ref<EditorCamera>& camera)
	{
		VT_PROFILE_FUNCTION(); 
		if (m_SelectedEntity) 
		{
			ImGuizmo::BeginFrame();
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 minRegion = ImGui::GetWindowContentRegionMin();
			ImVec2 maxRegion = ImGui::GetWindowContentRegionMax();
			float windowWidth = maxRegion.x - minRegion.x;
			float windowHeight = maxRegion.y - minRegion.y;
            ImGuizmo::SetRect(windowPos.x + minRegion.x, windowPos.y + minRegion.y, windowWidth, windowHeight);

			glm::mat4 cameraView = camera->GetViewMatrix();
			glm::mat4 cameraProjection = camera->GetProjection();
			cameraProjection[1][1] *= -1.0f;

			auto& tc = m_SelectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.ToMatrix();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, glm::value_ptr(transform));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

				tc.Translation = translation;
				tc.SetEulerDegrees(rotation);
				tc.Scale = scale;
			}
		}
	}

}