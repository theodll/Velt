#include "vtpch.h"
#include "IndexBuffer.h"
#include "Velt/Platform/Vulkan/VulkanIndexBuffer.h"
#include "Renderer.h"
#include <memory>

namespace Velt::Renderer
{


	std::shared_ptr<IndexBuffer> Create(u64 size)
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    return nullptr;
		case RendererAPI::Vulkan:  return std::make_shared<Vulkan::VulkanIndexBuffer>(size);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	
	std::shared_ptr<IndexBuffer> Create(void* data, u64 size = 0)
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    return nullptr;
		case RendererAPI::Vulkan:  return std::make_shared<Vulkan::VulkanIndexBuffer>(data, size);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}


}