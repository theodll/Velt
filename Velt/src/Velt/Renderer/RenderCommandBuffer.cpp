#include "RenderCommandBuffer.h"
#include "Renderer.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"

namespace Velt::Renderer 
{
	Ref<RenderCommandBuffer> RenderCommandBuffer::Create()
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:    return nullptr;
			case RenderAPI::API::Vulkan:  return CreateRef<RHI::VulkanCommandBuffer>();
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}