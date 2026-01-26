#include "vtpch.h"
#include "VertexBuffer.h"
#include "RenderAPI.h"
#include "Platform/Vulkan/Buffer/VulkanUniformBuffer.h"
#include "Velt/Renderer/Renderer.h"
#include "RenderAPI.h"

namespace Velt::Renderer
{

	std::shared_ptr<UniformBuffer> UniformBuffer::Create(u64 size, u64 offset)
	{
		VT_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    return nullptr;
		case RenderAPI::API::Vulkan:  return std::make_shared<RHI::VulkanUniformBuffer>(size, false);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}