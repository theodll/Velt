#include "vtpch.h"
#include "RenderPass.h"

#include "Renderer.h"

#include "Velt/Platform/Vulkan/VulkanRenderPass.h"

#include "Velt/Renderer/RendererAPI.h"

namespace Velt::Renderer {

	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:    assert(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::Vulkan:  return Ref<VulkanRenderPass>::Create(spec);
		}

		assert(false, "Unknown RendererAPI!");
		return nullptr;
	}

}