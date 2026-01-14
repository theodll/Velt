#include "SceneRenderer.h"

namespace Velt::Renderer {
	
	Ref<Vulkan::VulkanPipeline> SceneRenderer::s_Pipeline = nullptr;

	void SceneRenderer::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Init SceneRenderer");

		PipelineSpecification specs{};
		specs.FragmentShaderPath = { "" };
		specs.VertexShaderPath = {};
		specs.Layout = {};

		s_Pipeline = CreateRef<Vulkan::VulkanPipeline>(specs);
	}

	void SceneRenderer::Shutdown()
	{

	}

	void SceneRenderer::BeginScene()
	{

	}

	void SceneRenderer::EndScene()
	{

	}

	Ref<Pipeline> SceneRenderer::GetPipeline()
	{
		return nullptr;
	}

	VkRenderPass SceneRenderer::GetFinalRenderpass()
	{
		return VK_NULL_HANDLE;
	}

}