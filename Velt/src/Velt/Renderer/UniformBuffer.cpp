#include "vtpch.h"
#include "VertexBuffer.h"
#include "RenderAPI.h"
#include "Platform/Vulkan/Buffer/VulkanUniformBuffer.h"
#include "Velt/Renderer/Renderer.h"
#include "RenderAPI.h"

namespace Velt::Renderer
{

	std::shared_ptr<UniformBuffer> UniformBuffer::Create(void* uniformData, bool useStaging, u64 offset)
	{
		VT_PROFILE_FUNCTION();

		u64 bytes = vertexCount * vertexStride;
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    return nullptr;
		case RenderAPI::API::Vulkan:  return std::make_shared<RHI::VulkanUniformBuffer>(uniformData, false, offset);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}