#include "vtpch.h"
#include "VertexBuffer.h"
#include "RenderAPI.h"
#include "Platform/Vulkan/Buffer/VulkanVertexBuffer.h"
#include "Velt/Renderer/Renderer.h"
#include "RenderAPI.h"

namespace Velt::Renderer
{
    
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(void* vertexData, u64 vertexCount, u64 vertexStride, bool autoupload)
	{
		VT_PROFILE_FUNCTION();

		u64 bytes = vertexCount * vertexStride;
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:    return nullptr;
			case RenderAPI::API::Vulkan:  return std::make_shared<RHI::VulkanVertexBuffer>(vertexData, bytes, 0, autoupload);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}