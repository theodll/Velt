#include "Pipeline.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Renderer.h"
#include "RenderAPI.h"

namespace Velt
{
	Ref<Pipeline> Velt::Pipeline::Create(const PipelineSpecification& spec)
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RenderAPI::API::None:    return nullptr;
			case RenderAPI::API::Vulkan:  return  CreateRef<RHI::VulkanPipeline>(spec);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;

	}

}
