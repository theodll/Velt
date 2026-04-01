#pragma once
#include "Core/Core.h"

namespace Velt::Editor 
{
	class EditorPanel 
	{
	public: 
		virtual void Init() {};
		virtual void Shutdown() {}; 


		virtual void OnImGuiRender() {};
		virtual void OnImGuiRender2() {};
	};
}