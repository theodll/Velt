#include "vtpch.h"
#include "Core/Input.h"
#include "SDLInput.h"
#include "Core/Scancodes.h"

#include <SDL3/SDL.h>

namespace Velt::SDL
{
	bool SDLInput::IsKeyPressedImpl(Scancode code)
	{
		VT_PROFILE_FUNCTION();
		return s_Pressed[(size_t)code];
	}

	bool SDLInput::IsKeyReleasedImpl(Scancode code)
	{
		VT_PROFILE_FUNCTION();
		return s_Released[(size_t)code];
	}

	bool SDLInput::IsKeyDownImpl(Scancode code)
	{
		VT_PROFILE_FUNCTION();
		return s_Down[(size_t)code];
	}

	void SDLInput::ProcessEventImpl(const SDL_Event* pEvent)
	{
		VT_PROFILE_FUNCTION();
		switch (pEvent->type)
		{
		case SDL_EVENT_KEY_DOWN:
		{
			HandleKeyDown(pEvent);
			break;
		}
		case SDL_EVENT_KEY_UP:
		{
			HandleKeyUp(pEvent);
			break;
		};
		}
	}

	void SDLInput::BeginFrameImpl()
	{
		VT_PROFILE_FUNCTION();

		s_Pressed.fill(false);
		s_Released.fill(false);
		s_Down.fill(false);
	}

	void SDLInput::HandleKeyDown(const SDL_Event* pEvent)
	{
		VT_PROFILE_FUNCTION();
		if (!pEvent->key.repeat)
		{
			SDL_Scancode sc = pEvent->key.scancode;

			if (!s_Down[sc])
				s_Pressed[sc] = true;

			s_Down[sc] = true;
		}
	}

	void SDLInput::HandleKeyUp(const SDL_Event* pEvent)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Key Up Handeled");
		SDL_Scancode sc = pEvent->key.scancode;

		if (s_Down[sc])
			s_Released[sc] = true;

		s_Down[sc] = false;
	}

	float SDLInput::GetMouseXImpl()
	{
		float x{};
		SDL_GetMouseState(&x, VT_NULL_HANDLE);
		return x; 
	}

	float SDLInput::GetMouseYImpl()
	{
		float y{};
		SDL_GetMouseState(VT_NULL_HANDLE, &y);
		return y;
	}

	bool SDLInput::IsMouseKeyPressedImpl(MouseButton button)
	{
		u32 buttonState = SDL_GetMouseState(VT_NULL_HANDLE, VT_NULL_HANDLE);
		return buttonState & SDL_BUTTON_MASK(button);
	}

	bool SDLInput::IsMouseKeyReleasedImpl(MouseButton button)
	{
		u32 buttonState = SDL_GetMouseState(VT_NULL_HANDLE, VT_NULL_HANDLE);
		return buttonState & SDL_BUTTON_MASK(button);
	}

	bool SDLInput::IsMouseKeyDownImpl(MouseButton button)
	{
		u32 buttonState = SDL_GetMouseState(VT_NULL_HANDLE, VT_NULL_HANDLE);
		return buttonState & SDL_BUTTON_MASK(button);
	}

	void SDLInput::LockMouseImpl() 
	{
		SDL_HideCursor();
	};

	void SDLInput::UnlockMouseImpl() 
	{
		SDL_ShowCursor();
	};
	void SDLInput::TriggerMouseLockImpl() 
	{
		m_bIsMouseLock = SDL_CursorVisible();
		
		m_bIsMouseLock ? Velt::Input::LockMouse() : Velt::Input::UnlockMouse();
		
	};
}