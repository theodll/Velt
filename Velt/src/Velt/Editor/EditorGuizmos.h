#pragma once
#include "Core/Core.h"
#include "Core/Timestep.h"
#include "Renderer/Texture.h"

namespace Velt::Editor 
{
	class EditorGuizmos 
	{
	public: 
		EditorGuizmos() = default;
		virtual ~EditorGuizmos() = default;

		virtual void Init();
		virtual void Shutdown();

		virtual void OnUpdate(Timestep ts);

		virtual void OnImGuiRender();
		virtual void OnImGuiRender2();
	private: 
		Ref<Texture2D> m_MousePickingRenderTarget;
	};
}
