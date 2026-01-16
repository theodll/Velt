#include "SceneRenderer.h"

namespace Velt::Renderer {
	
	Ref<Pipeline> SceneRenderer::s_Pipeline = nullptr;

	void SceneRenderer::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Init SceneRenderer");

		PipelineSpecification specs{};
		specs.FragmentShaderPath = { "../../../../Shaders/fragment.glsl.spv" };
		specs.VertexShaderPath = { "../../../../Shaders/vertex.glsl.spv" };
		specs.Layout = {};

		s_Pipeline = Pipeline::Create(specs);
		s_Pipeline->Init();
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