#include "vtpch.h"
#include "IndexBuffer.h"
#include "Velt/Platform/Vulkan/Buffer/VulkanIndexBuffer.h"
#include "Renderer.h"
#include "RenderAPI.h"									 
#include <memory>										 
														 
namespace Velt::Renderer								 
{														 
														 
														 
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(u64 size)
	{													 
		VT_PROFILE_FUNCTION();							 
		switch (Renderer::GetAPI())						 
		{												 
		case RenderAPI::API::None:    return nullptr;	 
		case RenderAPI::API::Vulkan:  return std::make_shared<RHI::VulkanIndexBuffer>(size);
		}												 
		VT_CORE_ASSERT(false, "Unknown RendererAPI");	 
		return nullptr;									 
	}
	
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(void* data, u64 size = 0, u64 offset, bool autoupload)
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    return nullptr;
		case RenderAPI::API::Vulkan:  return std::make_shared<RHI::VulkanIndexBuffer>(data, size, offset, autoupload);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}


}
