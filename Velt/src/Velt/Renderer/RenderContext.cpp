#include "vtpch.h"
#include "RenderContext.h"
#include "Renderer.h"
#include "Platform/Vulkan/VulkanContext.h"

#include "RenderAPI.h"

namespace Velt::Renderer
{

	std::unique_ptr<Context> Context::Create()
	{		
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    return nullptr;
		case RenderAPI::API::Vulkan:  return std::make_unique<RHI::VulkanContext>();
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
		
	}

}