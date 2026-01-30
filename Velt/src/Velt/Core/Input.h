#pragma once
#include "Core/Core.h"
#include "Scancodes.h"
#include <SDL3/SDL.h>

namespace Velt
{
	class VELT_API Input
	{
	public:
		static void Init();

		static bool IsKeyDown(Scancode code) { if (s_Instance) return s_Instance->IsKeyDownImpl(code); else return false; }
		static bool IsKeyPressed(Scancode code) { if (s_Instance) return s_Instance->IsKeyPressedImpl(code); else return false; }
		static bool IsKeyReleased(Scancode code) { if (s_Instance) return s_Instance->IsKeyReleasedImpl(code); else return false; }

		static void BeginFrame() { return s_Instance->BeginFrameImpl(); }
		static void ProcessEvent(const SDL_Event& e) { return s_Instance->ProcessEventImpl(e); }
	protected:
		virtual bool IsKeyDownImpl(Scancode code) = 0;
		virtual bool IsKeyPressedImpl(Scancode code) = 0;
		virtual bool IsKeyReleasedImpl(Scancode code) = 0;

		virtual void BeginFrameImpl() = 0;
		virtual void ProcessEventImpl(const SDL_Event& e) = 0;
	private:
		static Scope<Input> s_Instance;
	};


}