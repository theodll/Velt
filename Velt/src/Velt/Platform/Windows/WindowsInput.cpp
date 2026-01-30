#include "vtpch.h"
#include "Core/Input.h"
#include "WindowsInput.h"

#include <SDL3/SDL.h>

namespace Velt::Windows
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

	void Input::ProcessEventImpl(const SDL_Event& e)
	{
		VT_PROFILE_FUNCTION();
		switch (e.type)
		{
		case SDL_EVENT_KEY_DOWN:
		{
			HandleKeyDown(e);
			break;
		}
		case SDL_EVENT_KEY_UP:
		{
			HandleKeyUp(e);
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

	void Input::HandleKeyDown(const SDL_Event& e)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Key Down Handeled"); 
		if (!e.key.repeat)
		{
			SDL_Scancode sc = e.key.scancode;

			if (!s_Down[sc])
				s_Pressed[sc] = true;

			s_Down[sc] = true;
		}
	}

	void Input::HandleKeyUp(const SDL_Event& e)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Key Up Handeled");
		SDL_Scancode sc = e.key.scancode;

		if (s_Down[sc])
			s_Released[sc] = true;

		s_Down[sc] = false;
	}

}