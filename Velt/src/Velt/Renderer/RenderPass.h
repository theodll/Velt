#pragma once
#include "Velt/Core/Core.h"

namespace Velt::Renderer
{
	class RenderPass
	{
	public:
		RenderPass() = default;
		virtual ~RenderPass() = default;

		virtual void Init() = 0;

		static Ref<RenderPass> Create();
	};

	// blatanlty stole from hazel :D

}
