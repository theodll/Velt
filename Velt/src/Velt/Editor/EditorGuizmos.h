#pragma once
#include "Core/Core.h"
#include "Core/Timestep.h"
#include "Renderer/Texture.h"
#include "Panels/EditorViewportPanel.h"
#include "Events/Event.h"
#include <entt/entt.hpp>

namespace Velt::Editor 
{
	class VELT_API EditorGuizmos 
	{
	public: 
		EditorGuizmos() = default;
		virtual ~EditorGuizmos() = default;

		virtual void Init();
		virtual void Shutdown();

		virtual void OnUpdate(Timestep ts, const Ref<EditorViewportPanel>& editorPanel);
		virtual void OnEvent(Event& event);

		virtual void OnImGuiRender();
		virtual void OnImGuiRender2();
	private: 
		Ref<Texture2D> m_MousePickingRenderTarget;
		entt::entity m_SelectedEntity = entt::null;
	};
}
