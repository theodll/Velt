#pragma once
#include "Velt/Core/Core.h"

namespace Velt::Renderer
{
	class Context
	{
	public:
		Context() = default;
		virtual ~Context() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;
		virtual void DrawFrame() = 0;

		static Context* Create();
	};
}

