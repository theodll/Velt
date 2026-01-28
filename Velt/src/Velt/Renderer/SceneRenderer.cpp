#include "SceneRenderer.h"
#include "Buffer.h"
#include "Core/Core.h"
#include "Core/Application.h"

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
		
		float width, height{};

		if (ImGuiLayer::GetViewport())
		{
			width = ImGuiLayer::GetViewport()->GetWidth();
			height = ImGuiLayer::GetViewport()->GetHeight();
		} 
		else
		{
			width = 1920;
			height = 1080;
		}

		float aspect = width / height;

		float halfHeight = 1.0f;
		float halfWidth = aspect * halfHeight;

		m_Camera = CreateRef<OrthographicCamera>(
			-halfWidth,
			halfWidth,
			-halfHeight,
			halfHeight
		);
		m_Camera->SetRotation(0);

		auto& sc = Velt::Application::Get().GetWindow().GetSwapchain();

		const u32 mfif = sc.GetMaxFrameInFlight();
		m_CameraUBOs.resize(mfif);
		for (u32 i = 0; i < mfif; i++)
		{
			m_CameraUBOs[i] = UniformBuffer::Create(sizeof(CameraUBO));
			s_Pipeline->UpdateDescriptorSet(i, 0, m_CameraUBOs[i]);
		}
	}

	void SceneRenderer::Shutdown()
	{

	}

	void SceneRenderer::BeginScene()
	{
		VT_PROFILE_FUNCTION();
		
		float width, height{};

		if (ImGuiLayer::GetViewport())
		{
			width = ImGuiLayer::GetViewport()->GetWidth();
			height = ImGuiLayer::GetViewport()->GetHeight();
		}
		else
		{
			width = 1920;
			height = 1080;
		}

		float aspect = width / height;

		float halfHeight = 1.0f;
		float halfWidth = aspect * halfHeight;

		m_Camera->SetProjection(
			-halfWidth,
			halfWidth,
			-halfHeight,
			halfHeight
		);
		// m_Camera->SetRotation(m_Rotation);
		m_Rotation++;
		auto cmd = Velt::Application::Get().GetWindow().GetSwapchain().GetCurrentDrawCommandBuffer();
		auto frameIndex = Velt::Application::Get().GetWindow().GetSwapchain().GetCurrentFrameIndex();

		CameraUBO ubo{};
		ubo.viewProj = m_Camera->GetViewProjectionMatrix();

		m_CameraUBOs[frameIndex]->SetData(&ubo, sizeof(CameraUBO), 0);
		
		s_Pipeline->Bind(cmd);
		s_Pipeline->BindDescriptorSet(cmd, frameIndex);
	}

	void SceneRenderer::EndScene()
	{
		VT_PROFILE_FUNCTION();
	}

}