#include "vtpch.h"
#include "VertexBuffer.h"
#include "RendererAPI.h"
#include "Platform/Vulkan/VulkanVertexBuffer.h"
#include "Velt/Renderer/Renderer.h"

namespace Velt::Renderer
{
    
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* vertexData, u64 vertexCount, u64 vertexStride)
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:    return nullptr;
			case RendererAPI::Vulkan:  return std::make_shared<Vulkan::VulkanVertexBuffer>(vertexData, vertexCount, vertexStride);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}