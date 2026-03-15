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

		static float GetMouseX() { if (s_Instance) return s_Instance->GetMouseXImpl(); else return 0.0f;  }
		static float GetMouseY() { if (s_Instance) return s_Instance->GetMouseYImpl(); else return 0.0f; }

		static bool IsMouseKeyPressed(MouseButton button) { if (s_Instance) return s_Instance->IsMouseKeyPressedImpl(button); else return false; }
		static bool IsMouseKeyReleased(MouseButton button) { if (s_Instance) return s_Instance->IsMouseKeyReleasedImpl(button); else return false; }
		static bool IsMouseKeyDown(MouseButton button) { if (s_Instance) return s_Instance->IsMouseKeyDownImpl(button); else return false; }

		static void BeginFrame() { return s_Instance->BeginFrameImpl(); }
		static void ProcessEvent(const SDL_Event* pEvent) { return s_Instance->ProcessEventImpl(pEvent); }

	protected:
		virtual bool IsKeyDownImpl(Scancode code) = 0;
		virtual bool IsKeyPressedImpl(Scancode code) = 0;
		virtual bool IsKeyReleasedImpl(Scancode code) = 0;

		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

		virtual bool IsMouseKeyPressedImpl(MouseButton button) = 0;
		virtual bool IsMouseKeyReleasedImpl(MouseButton button) = 0;
		virtual bool IsMouseKeyDownImpl(MouseButton button) = 0;

		virtual void BeginFrameImpl() = 0;
		virtual void ProcessEventImpl(const SDL_Event* pEvent) = 0;
	private:
		static Scope<Input> s_Instance;
	};


}