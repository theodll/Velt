#include "SceneRenderer.h"
#include "Buffer.h"
#include "Core/Core.h"
#include "Platform/Vulkan/DescriptorLayoutCache.h"
#include "Core/Application.h"
#include "Core/Input.h"
#include "PipelineManager.h"

namespace Velt {
	
	Ref<Pipeline> SceneRenderer::s_Pipeline = nullptr;
	Ref<Camera> SceneRenderer::m_Camera = nullptr;

	void SceneRenderer::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Init SceneRenderer");

		BufferLayout layout
		{
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_UV"}
		};

		PipelineManager::Init();

		// Todo [25.02, Theo]: Move this somewhere else 

		std::vector<RHI::DescriptorBinding> globalBindings{};
		RHI::DescriptorBinding viewProj{};
		viewProj.type = RHI::DescriptorType::UNIFORM_BUFFER;
		viewProj.binding = 0;
		viewProj.count = 1;
		viewProj.stage = RHI::ShaderStage::VERTEX;

		globalBindings.emplace_back(viewProj);

		std::vector<RHI::DescriptorBinding> materialBindings{};
		RHI::DescriptorBinding color{};
		color.type = RHI::DescriptorType::UNIFORM_BUFFER;
		color.binding = 0;
		color.count = 1;
		color.stage = RHI::ShaderStage::FRAGMENT;
		materialBindings.emplace_back(color);

		RHI::DescriptorBinding albedoTexture{};
		albedoTexture.type = RHI::DescriptorType::COMBINED_IMAGE_SAMPLER;
		albedoTexture.binding = 1; 
		albedoTexture.count = 1;
		albedoTexture.stage = RHI::ShaderStage::FRAGMENT;
		materialBindings.emplace_back(albedoTexture);
		
		auto globalLayout = RHI::VulkanContext::GetLayoutCache()->CreateLayout(&globalBindings);
		auto materialLayout = RHI::VulkanContext::GetLayoutCache()->CreateLayout(&materialBindings);

		{
			std::vector<DescriptorSetLayoutHandle> setLayouts;
			setLayouts.emplace_back(globalLayout);
			setLayouts.emplace_back(materialLayout);

			PipelineSpecification specs{};
			specs.VertexShader = ShaderLibrary::Get("Assets/Shader/basic_vertex_shader.hlsl.spv");
			specs.FragmentShader = ShaderLibrary::Get("Assets/Shader/basic_fragment_shader.hlsl.spv");
			specs.SetLayouts = setLayouts;
			specs.Layout = layout;


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

		float halfHeight = 1.0f;
		float halfWidth = aspect * halfHeight;

		m_Camera = CreateRef<Camera>();
		const auto& sc = Velt::Application::Get()->GetWindow()->GetSwapchain();

		const u32 mfif = sc->GetMaxFrameInFlight();
		m_CameraUBOs.resize(mfif);
		m_GlobalSets.resize(mfif);
		for (u32 i = 0; i < mfif; i++)
		{
			m_CameraUBOs[i] = UniformBuffer::Create(sizeof(CameraUBO));
			m_GlobalSets[i] = RHI::VulkanContext::GetSetManager()->Allocate(globalLayout);
			

			RHI::VulkanContext::GetSetManager()->WriteBuffer(
				m_GlobalSets[i],
				0,
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

		m_Camera->SetPerspectiveProjection(glm::radians(50.0f), aspectF, 0.1f, 1000.0f);

		// m_Camera->SetRotation(m_Rotation);
		m_Rotation++;
		auto cmd = Velt::Application::Get()->GetWindow()->GetSwapchain()->GetCurrentDrawCommandBuffer();
		auto frameIndex = Velt::Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex();
		auto pipelineLayout = s_Pipeline->GetVulkanPipelineLayout();

		CameraUBO ubo{};
		ubo.viewProj = m_Camera->GetProjectionMatrix() * m_Camera->GetViewMatrix();

		m_CameraUBOs[frameIndex]->SetData(&ubo, sizeof(CameraUBO), 0);

		s_Pipeline->Bind(cmd);

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