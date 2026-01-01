#include "RenderContext.h"
#include "Renderer.h"
#include "Platform/Vulkan/VulkanContext.h"

namespace Velt::Renderer
{

	Context* Context::Create()
	{		
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:    return nullptr;
		case RendererAPI::Vulkan:  return new Vulkan::VulkanContext;
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
		
	}

}