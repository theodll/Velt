#include "vtpch.h"

#include "Input.h"
#include "Platform/SDL/SDLInput.h"

namespace Velt
{
	Scope<Input> Input::s_Instance = nullptr;

	void Input::Init()
	{
		s_Instance = CreateScope<SDL::Input>();
	};
}