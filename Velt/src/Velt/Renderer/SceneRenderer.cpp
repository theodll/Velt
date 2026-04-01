
#include "vtpch.h"
#include "Core/Core.h"
#include "Core/Application.h"
#include "Core/Input.h"

#include "SceneRenderer.h"
#include "Buffer.h"
#include "Platform/Vulkan/DescriptorLayoutCache.h"
#include "PipelineManager.h"
#include "BindingLayouts.h"
#include "Scene/Scene.h"

namespace Velt {
	
	Ref<Pipeline> SceneRenderer::s_GeometryPipeline = nullptr;

	void SceneRenderer::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Init SceneRenderer");

		PipelineManager::Init();

		{
			BufferLayout geometryLayout
			{
					{ ShaderDataType::Float3, "a_Position" },
					{ ShaderDataType::Float3, "a_Normal" },
					{ ShaderDataType::Float3, "a_Tangent" },
					{ ShaderDataType::Float3, "a_Binormal" },
					{ ShaderDataType::Float2, "a_UV"}
			};

			auto gVertexShader = ShaderLibrary::Get("Assets/Shader/gbuffer_vertex.hlsl.spv");
			auto gPixelShader = ShaderLibrary::Get("Assets/Shader/gbuffer_pixel.hlsl.spv");

			static PipelineSpecification geometryPipelineSpecs{};
			geometryPipelineSpecs.VertexShader = gVertexShader;
			geometryPipelineSpecs.FragmentShader = gPixelShader;
			geometryPipelineSpecs.Layout = geometryLayout;
			geometryPipelineSpecs.ColorAttachmentFormats = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_SRGB };
			geometryPipelineSpecs.CullMode = VT_CULL_MODE_BACK_BIT;

			m_CameraUBOBinding = VT_CAMERA_SET_UBO_BINDING;

			s_GeometryPipeline = Pipeline::Create(&geometryPipelineSpecs);
			s_GeometryPipeline->Init();
		}
		
		const auto& sc = Velt::Application::Get()->GetWindow()->GetSwapchain();

		m_CameraUBOs.resize(MAX_FRAMES_IN_FLIGHT);
		m_GlobalSets.resize(MAX_FRAMES_IN_FLIGHT);
		const auto& setLayouts = s_GeometryPipeline->GetSetLayouts();
		VT_CORE_ASSERT(!setLayouts.empty(), "");
		for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_CameraUBOs[i] = UniformBuffer::Create(sizeof(CameraUBO));
			m_GlobalSets[i] = RHI::VulkanContext::GetSetManager()->Allocate(setLayouts[0]);
			

			RHI::VulkanContext::GetSetManager()->WriteBuffer(
				m_GlobalSets[i],
				m_CameraUBOBinding,
				m_CameraUBOs[i]->GetVulkanBuffer(),
				sizeof(CameraUBO)
			);
		
		}
	}

	void SceneRenderer::Shutdown()
	{
		VT_CORE_INFO("Shutdown Scene Renderer");
		PipelineManager::Shutdown();
		s_GeometryPipeline->Shutdown();

		s_GeometryPipeline.reset();
		m_CameraUBOs.clear();
	}

	glm::vec2 GetAspectPair(u32 width, u32 height, float targetY = 0.9f)
	{
		VT_PROFILE_FUNCTION();

		float w = (float)width;
		float h = (float)height;

		float scale = targetY / h; 

	// 	VT_CORE_TRACE("GetAspectPair w: {0}, h: {1}, wa: {2}, ha {3}", width, height, w * scale, h * scale);

		return { w * scale, h * scale };
	}

	void SceneRenderer::BeginScene(const Ref<Camera>& pCamera)
	{
		VT_PROFILE_FUNCTION();
		
		u32 w = Application::Get()->GetRenderableWidth();
		u32 h = Application::Get()->GetRenderableHeight();

		glm::vec2 aspect = GetAspectPair(w, h);

		float aspectF = w / h;
		
		auto cmd = Velt::Application::Get()->GetWindow()->GetSwapchain()->GetCurrentDrawCommandBuffer();
		auto frameIndex = Velt::Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex();
		auto pipelineLayout = s_GeometryPipeline->GetVulkanPipelineLayout();

		CameraUBO ubo{};
		ubo.viewProj = pCamera->GetViewProjection();
		ubo.invViewProj = pCamera->GetInverseViewProjection();
		ubo.cameraPos = pCamera->GetPosition();

		m_CameraUBOs[frameIndex]->SetData(&ubo, sizeof(CameraUBO), 0);

		// Todo [25.02, Theo]: Make this platform independent
		vkCmdBindDescriptorSets(
			cmd,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0,
			1,
			&m_GlobalSets[frameIndex],
			0,
			nullptr
		);

	}

	void SceneRenderer::EndScene()
	{
		VT_PROFILE_FUNCTION();
	}

}