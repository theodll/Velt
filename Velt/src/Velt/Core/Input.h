#pragma once
#include "Core/Core.h"
#include "Scancodes.h"

namespace Velt
{
	class VELT_API Input
	{
	public:
		static bool IsKeyPressed(Scancode code) { return s_Instance->IsKeyPressedImpl(code); };
	protected:
		virtual bool IsKeyPressedImpl(Scancode code) = 0;
	private:
		static Input* s_Instance;
	};


}