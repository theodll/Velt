#include "SceneRenderer.h"
#include "Buffer.h"

namespace Velt::Renderer {
	
	Ref<Pipeline> SceneRenderer::s_Pipeline = nullptr;

	void SceneRenderer::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Init SceneRenderer");

		BufferLayout layout{

				{ ShaderDataType::Float2, "a_Position" },
				{ ShaderDataType::Float3, "a_Color" }
		};
		

		PipelineSpecification specs{};
		specs.FragmentShaderPath = { "Shaders/fragment.glsl.spv" };
		specs.VertexShaderPath = { "Shaders/vertex.glsl.spv" };
		specs.Layout = layout;

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

	VkRenderPass SceneRenderer::GetFinalRenderpass()
	{
		return VK_NULL_HANDLE;
	}

}