#pragma once

#include "Velt/Core/Core.h"
#include "Pipeline.h"

namespace Velt {

	class RenderCommandBuffer
	{
	public:
		virtual ~RenderCommandBuffer() = default;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Submit() = 0;

		virtual VkCommandBuffer GetVulkanCommandBuffer() = 0;

		static Ref<RenderCommandBuffer> Create();
	};

}