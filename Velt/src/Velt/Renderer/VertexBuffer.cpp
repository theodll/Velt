#include "vtpch.h"
#include "VertexBuffer.h"
#include "RenderAPI.h"
#include "Platform/Vulkan/Buffer/VulkanVertexBuffer.h"
#include "Velt/Renderer/Renderer.h"
#include "RenderAPI.h"

namespace Velt::Renderer
{
    
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* vertexData, u64 vertexCount, u64 vertexStride)
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:    return nullptr;
			case RenderAPI::API::Vulkan:  return std::make_shared<Vulkan::VulkanVertexBuffer>(vertexData, vertexCount, vertexStride);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}