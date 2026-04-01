#pragma once
#include "Core/Core.h"
#include "Core/Timestep.h"

namespace Velt::Editor 
{
	class EditorPanel 
	{
	public: 
		EditorPanel() = default;
		virtual ~EditorPanel() = default;

		virtual void Init() {};
		virtual void Shutdown() {}; 

		virtual void OnUpdate(Timestep ts) {};

		virtual void OnImGuiRender() {};
		virtual void OnImGuiRender2() {};
	};
}