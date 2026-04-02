#pragma once
#include "Core/Core.h"
#include "Core/Timestep.h"

namespace Velt::Editor 
{
	class EditorGuizmo 
	{
	public: 
		EditorGuizmo() = default;
		virtual ~EditorGuizmo() = default;

		virtual void Init() {};
		virtual void Shutdown() {};

		virtual void OnUpdate(Timestep ts) {};

		virtual void OnImGuiRender() {};
		virtual void OnImGuiRender2() {};
	};
}
