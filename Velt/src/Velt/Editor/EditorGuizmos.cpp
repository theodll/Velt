#include "vtpch.h"
#include "EditorGuizmos.h"
#include "Renderer/Renderer.h"

namespace Velt::Editor 
{
	void EditorGuizmos::Init()
	{
		m_MousePickingRenderTarget = Renderer::GetRenderTarget(VT_RENDER_TARGET_MOUSE_PICKING);
	}

	void EditorGuizmos::Shutdown()
	{

	}

	void EditorGuizmos::OnImGuiRender2()
	{

	}

	void EditorGuizmos::OnImGuiRender()
	{

	}

	void EditorGuizmos::OnUpdate(Timestep ts)
	{

	}

}