#include "RenderCommandBuffer.h"
#include "Renderer.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"

namespace Velt::Renderer 
{
	Ref<RenderCommandBuffer> RenderCommandBuffer::Create(u32 count = 0, const std::string& debugName = "")
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:    return nullptr;
			//case RenderAPI::API::Vulkan:  return std::make_shared<Vulkan::VulkanCommandBuffer>(vertexData, vertexCount, vertexStride);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}