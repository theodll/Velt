#include "Pipeline.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Renderer.h"

namespace Velt::Renderer
{
	Pipeline* Velt::Renderer::Pipeline::Create(const PipelineSpecification& spec)
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:    return nullptr;
			case RendererAPI::Vulkan:  return new Vulkan::VulkanPipeline(spec);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

}
