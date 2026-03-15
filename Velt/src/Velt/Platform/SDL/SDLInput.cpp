#include "vtpch.h"
#include "Core/Input.h"
#include "SDLInput.h"
#include "Core/Scancodes.h"

#include <SDL3/SDL.h>

namespace Velt::SDL
{
	bool Input::IsKeyPressedImpl(Scancode code)
	{
		VT_PROFILE_FUNCTION();
		return s_Pressed[(size_t)code];
	}

	bool Input::IsKeyReleasedImpl(Scancode code)
	{
		VT_PROFILE_FUNCTION();
		return s_Released[(size_t)code];
	}

	bool Input::IsKeyDownImpl(Scancode code)
	{
		VT_PROFILE_FUNCTION();
		return s_Down[(size_t)code];
	}

	void Input::ProcessEventImpl(const SDL_Event* pEvent)
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

	void Input::BeginFrameImpl()
	{
		VT_PROFILE_FUNCTION();

		s_Pressed.fill(false);
		s_Released.fill(false);
		s_Down.fill(false);
	}

	void Input::HandleKeyDown(const SDL_Event* pEvent)
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

	void Input::HandleKeyUp(const SDL_Event* pEvent)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Key Up Handeled");
		SDL_Scancode sc = pEvent->key.scancode;

		if (s_Down[sc])
			s_Released[sc] = true;

		s_Down[sc] = false;
	}

	float Input::GetMouseXImpl()
	{
		float x{};
		SDL_GetMouseState(&x, VT_NULL_HANDLE);
		return x; 
	}

	float Input::GetMouseYImpl()
	{
		float y{};
		SDL_GetMouseState(VT_NULL_HANDLE, &y);
		return y;
	}

	bool Input::IsMouseKeyPressedImpl(MouseButton button)
	{
		u32 buttonState = SDL_GetMouseState(VT_NULL_HANDLE, VT_NULL_HANDLE);
		return buttonState & SDL_BUTTON_MASK(button);
	}

	bool Input::IsMouseKeyReleasedImpl(MouseButton button)
	{
		u32 buttonState = SDL_GetMouseState(VT_NULL_HANDLE, VT_NULL_HANDLE);
		return buttonState & SDL_BUTTON_MASK(button);
	}

	bool Input::IsMouseKeyDownImpl(MouseButton button)
	{
		u32 buttonState = SDL_GetMouseState(VT_NULL_HANDLE, VT_NULL_HANDLE);
		return buttonState & SDL_BUTTON_MASK(button);
	}

}