#include "SceneRenderer.h"
#include "Buffer.h"
#include "Core/Core.h"
#include "Platform/Vulkan/DescriptorLayoutCache.h"
#include "Core/Application.h"
#include "Core/Input.h"
#include "PipelineManager.h"

namespace Velt {
	
	Ref<Pipeline> SceneRenderer::s_Pipeline = nullptr;
	Ref<EditorCamera> SceneRenderer::m_Camera = nullptr;

	void SceneRenderer::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Init SceneRenderer");

		BufferLayout layout
		{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float3, "a_Normal" },
				{ ShaderDataType::Float3, "a_Tangent" },
				{ ShaderDataType::Float3, "a_Binormal" },
				{ ShaderDataType::Float2, "a_UV"}
		};

		PipelineManager::Init();

		{
			PipelineSpecification specs{};
		//	specs.VertexShader = ShaderLibrary::Get("Assets/Shader/basic_vertex_shader.hlsl.spv");
		//	specs.FragmentShader = ShaderLibrary::Get("Assets/Shader/basic_fragment_shader.hlsl.spv");
			auto vertexShader = ShaderLibrary::Get("Assets/Shader/gbuffer_vertex.hlsl.spv");
			auto fragmentShader = ShaderLibrary::Get("Assets/Shader/gbuffer_pixel.hlsl.spv");
			specs.VertexShader = vertexShader;
			specs.FragmentShader = fragmentShader;
			specs.Layout = layout;

			m_ViewProjBinding = 0;
			bool foundViewProj = false;
			auto setIt = vertexShader->ReflectData.find(0);
			if (setIt != vertexShader->ReflectData.end())
			{
				for (const auto& b : setIt->second.Bindings)
				{
					if (b.type == RHI::DescriptorType::UNIFORM_BUFFER)
					{
						m_ViewProjBinding = b.binding;
						foundViewProj = true;
						break;
					}
				}
			}
			VT_CORE_ASSERT(foundViewProj, "");

			s_Pipeline = Pipeline::Create(&specs);
			s_Pipeline->Init();
		}
		
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


		m_Camera = CreateRef<EditorCamera>(glm::radians(50.0f), aspect, 0.1f, 1000.0f);
		const auto& sc = Velt::Application::Get()->GetWindow()->GetSwapchain();

		const u32 mfif = sc->GetMaxFrameInFlight();
		m_CameraUBOs.resize(mfif);
		m_GlobalSets.resize(mfif);
		const auto& setLayouts = s_Pipeline->GetSetLayouts();
		VT_CORE_ASSERT(!setLayouts.empty(), "");
		for (u32 i = 0; i < mfif; i++)
		{
			m_CameraUBOs[i] = UniformBuffer::Create(sizeof(CameraUBO));
			m_GlobalSets[i] = RHI::VulkanContext::GetSetManager()->Allocate(setLayouts[0]);
			

			RHI::VulkanContext::GetSetManager()->WriteBuffer(
				m_GlobalSets[i],
				m_ViewProjBinding,
				m_CameraUBOs[i]->GetVulkanBuffer(),
				sizeof(CameraUBO)
			);
		
		}
	}

	void SceneRenderer::Shutdown()
	{
		VT_CORE_TRACE("Shutdown Scene Renderer");
		PipelineManager::Shutdown();
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

	void SceneRenderer::BeginScene()
	{
		VT_PROFILE_FUNCTION();
		
		u32 w = ImGuiLayer::GetViewport()->GetWidth();
		u32 h = ImGuiLayer::GetViewport()->GetHeight();

		glm::vec2 aspect = GetAspectPair(w, h);

		float aspectF = w / h;
		// VT_CORE_ERROR("ax: {0} ay:, {1}", aspect.x, aspect.y);
		
		/*
		m_Camera->SetOrthographicProjection(
			-aspect.x,
			aspect.x,
			-aspect.y,
			aspect.y,
			-1,
			1
		);
		*/

//		m_Camera->SetPerspectiveProjection(glm::radians(50.0f), aspectF, 0.1f, 1000.0f);

		// m_Camera->SetRotation(m_Rotation);
		m_Rotation++;
		auto cmd = Velt::Application::Get()->GetWindow()->GetSwapchain()->GetCurrentDrawCommandBuffer();
		auto frameIndex = Velt::Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex();
		auto pipelineLayout = s_Pipeline->GetVulkanPipelineLayout();

		CameraUBO ubo{};
		ubo.viewProj = m_Camera->GetViewProjection();

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
	}

}