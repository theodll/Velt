#pragma once
#include "Velt/Core/Core.h"

namespace Velt::Renderer
{
	class RendererContext
	{
	public:
		RendererContext() = default;
		virtual ~RendererContext() = default;

		virtual void Init() = 0;

		static Ref<RendererContext> Create();
	};

	// blatanlty stole from hazel :D

}
