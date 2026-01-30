#include "vtpch.h"

#include "Input.h"
#include "Platform/Windows/WindowsInput.h"

namespace Velt
{
	Scope<Input> Input::s_Instance = nullptr;

	void Input::Init()
	{
		s_Instance = CreateScope<Windows::Input>();
	};
}