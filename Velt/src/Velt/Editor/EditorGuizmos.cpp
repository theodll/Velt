#include "vtpch.h"
#include "EditorGuizmos.h"
#include "Panels/EditorViewportPanel.h"
#include "Renderer/Renderer.h"
#include "Core/Input.h"

namespace Velt::Editor 
{
	void EditorGuizmos::Init()
	{
		VT_PROFILE_FUNCTION();
		m_MousePickingRenderTarget = Renderer::GetRenderTarget(VT_RENDER_TARGET_MOUSE_PICKING);
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

	void EditorGuizmos::OnUpdate(Timestep ts, const Ref<EditorViewportPanel>& editorPanel)
	{
		VT_PROFILE_FUNCTION();
		if (Input::IsMouseKeyPressed(VELT_MOUSE1)) 
		{
			auto&& image = Renderer::GetRenderTarget(VT_RENDER_TARGET_MOUSE_PICKING);
			u32 id = image->ReadPixel((u32)editorPanel->m_LocalMousePos.x, (u32)editorPanel->m_LocalMousePos.y);

			// 0xFFFFFFFF is the standart draw call value.
			// 0x3F800000 is the clear color represented by float 1.0f
			if (id == 0xFFFFFFFF || id == 0x3F800000)
				m_SelectedEntity = entt::null;
			else
				m_SelectedEntity = (entt::entity)id;
			
		}
	}

	void EditorGuizmos::OnEvent(Event& event)
	{
		VT_PROFILE_FUNCTION();
		

	}

}