#include "vtpch.h"

#include "Core/Input.h"

namespace Velt::Windows
{
	class Input : public Velt::Input
	{
	protected: 
		virtual bool IsKeyPressedImpl(Scancode code) override;
	};
}