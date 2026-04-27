#include "vtpch.h"
#include "DefferedRenderer.h"
#include "Renderer.h"
#include "Core/Application.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/DescriptorSetManager.h"
#include "BindingLayouts.h"
#include "Lights.h"
#include "../../../VeltEditor/src/EditorLayer.h"

namespace Velt 
{
	Ref<Pipeline> DefferedRenderer::s_DefferedPipeline = nullptr;

	void DefferedRenderer::Init(Ref<Camera> pCamera, const Ref<Scene>& pActiveScene)
	{
		VT_PROFILE_FUNCTION();
		{
			BufferLayout defferedLayout{};

			auto dPBRVertexShader = ShaderLibrary::Get("Assets/Shader/deffered_pbr_vertex.hlsl.spv");
			auto dPBRPixelShader = ShaderLibrary::Get("Assets/Shader/deffered_pbr_pixel.hlsl.spv");

//			auto dPBRVertexShader = ShaderLibrary::Get("Assets/Shader/basic_vertex_shader.hlsl.spv");
//			auto dPBRPixelShader = ShaderLibrary::Get("Assets/Shader/basic_fragment_shader.hlsl.spv");

			PipelineSpecification defferedPBRPipelineSpecification{};
			defferedPBRPipelineSpecification.VertexShader = dPBRVertexShader;
			defferedPBRPipelineSpecification.FragmentShader = dPBRPixelShader;
			defferedPBRPipelineSpecification.Layout = defferedLayout;
			defferedPBRPipelineSpecification.ColorAttachmentFormats = { VK_FORMAT_B8G8R8A8_SRGB };
			defferedPBRPipelineSpecification.CullMode = VT_CULL_MODE_NONE;

			s_DefferedPipeline = Pipeline::Create(&defferedPBRPipelineSpecification);
			s_DefferedPipeline->Init();
		}

		m_ShaderInput = CreateRef<DefferedShaderInput>(pCamera);
		SetActiveScene(pActiveScene);
	}

	void DefferedRenderer::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("Shutdown Deffered Renderer");
		s_DefferedPipeline->Shutdown();
		s_DefferedPipeline.reset();
	}

	void DefferedRenderer::SetActiveScene(const Ref<Scene>& pActiveScene)
	{
		VT_PROFILE_FUNCTION();

		VT_CORE_INFO("Set Deffered Renderer Scene");
		m_ShaderInput->m_ActiveScene = pActiveScene;
	}

	void DefferedRenderer::ExecuteDefferedPass(VkCommandBuffer cmd)
	{
		VT_PROFILE_FUNCTION();

		m_ShaderInput->UpdateData();
		Renderer::SubmitFullscreenTriangle(cmd, s_DefferedPipeline, m_ShaderInput);
	}

	DefferedShaderInput::DefferedShaderInput(Ref<Camera> pCamera)
	{
		VT_PROFILE_FUNCTION();
		auto pipeline = DefferedRenderer::GetPipeline();
		VT_CORE_ASSERT(pipeline, "");
		
		m_Camera = pCamera;

		m_AlbedoAOBinding = VT_DEFFERED_SET_BINDING_ALBEDO_AO;
		m_NormalRoughBinding = VT_DEFFERED_SET_BINDING_NORMAL_ROUGH;
		m_MetalEmitBinding = VT_DEFFERED_SET_BINDING_METAL_EMIT;
		m_DepthBinding = VT_DEFFERED_SET_BINDING_DEPTH;
		m_SamplerBinding = VT_DEFFERED_SET_BINDING_SAMPLER;
		m_CameraUBOBinding = VT_DEFFERED_SET_BINDING_CAMERA_BO;
		m_LightUBOBinding = VT_DEFFERED_SET_BINDING_LIGHT_UBO;

		
		m_ValidBindings.emplace(m_AlbedoAOBinding);
		m_ValidBindings.emplace(m_NormalRoughBinding);
		m_ValidBindings.emplace(m_MetalEmitBinding);
		m_ValidBindings.emplace(m_DepthBinding);
		m_ValidBindings.emplace(m_SamplerBinding);
		m_ValidBindings.emplace(m_CameraUBOBinding);
		m_ValidBindings.emplace(m_LightUBOBinding);

		m_CameraUBOs.resize(MAX_FRAMES_IN_FLIGHT);
		for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_GeometryBuffers[i] = CreateRef<GBuffer>();

			const auto& layouts = pipeline->GetSetLayouts();
			VT_CORE_ASSERT(layouts.size() > 2, "");

			// Note [27.02, Theo] The two stands for the Deferred Set Layout (set 0 = global, set 1 = material, set 2 = deferred)
			m_Sets[i] = RHI::VulkanContext::GetSetManager()->Allocate(layouts[2]);

			m_CameraUBOs[i] = UniformBuffer::Create(sizeof(CameraUBO));

			RHI::VulkanContext::GetSetManager()->WriteBuffer(
				m_Sets[i],
				m_CameraUBOBinding,
				m_CameraUBOs[i]->GetVulkanBuffer(),
				sizeof(CameraUBO)
			);


			RHI::VulkanContext::GetSetManager()->WriteBuffer(
				m_Sets[i],
				m_LightUBOBinding,
				m_LightUBOs[i]->GetVulkanBuffer(),
				sizeof(LightUBO)
			);


			if (m_TextureSampler) 
			{
				RHI::VulkanContext::GetSetManager()->WriteSampler(
					m_Sets[i],
					m_SamplerBinding,
					m_TextureSampler->GetSampler()
				);
			}
		}
	
		UpdateData();
	}

	void DefferedShaderInput::UpdateData()
	{
		VT_PROFILE_FUNCTION();
		auto&& renderTargets = Renderer::GetRenderTargets();
		auto frameIndex = Velt::Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex();
		/*
		if (!m_TextureSampler)
		{
			if (renderTargets[VT_RENDER_TARGET_SAMPLER]) {
				m_TextureSampler = renderTargets[VT_RENDER_TARGET_SAMPLER];
				RHI::VulkanContext::GetSetManager()->WriteSampler(
					m_Sets[frameIndex],
					m_SamplerBinding,
					m_TextureSampler->GetSampler()
				);
			}
		}
		*/
		if (!m_TextureSampler)
		{
			if (renderTargets[VT_RENDER_TARGET_SAMPLER]) {
				m_TextureSampler = renderTargets[VT_RENDER_TARGET_SAMPLER];
				for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				{
					RHI::VulkanContext::GetSetManager()->WriteSampler(
						m_Sets[i],
						m_SamplerBinding,
						m_TextureSampler->GetSampler()
					);
				}
			}
		}

		auto updateImageBinding = [&](u32 binding, Ref<Texture2D> pTexture)
		{
			if (m_ValidBindings.find(binding) == m_ValidBindings.end() || !pTexture)
				return;

			m_GeometryBuffers[frameIndex]->Targets[binding] = pTexture;

			VkDescriptorImageInfo info{};
			info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			info.imageView = pTexture->GetImageView();
			info.sampler = VT_NULL_HANDLE;

			RHI::VulkanContext::GetSetManager()->WriteImage(
				m_Sets[frameIndex],
				binding,
				info
			);
		};

		updateImageBinding(m_AlbedoAOBinding, renderTargets[VT_RENDER_TARGET_ALBEDO_AO]);
		updateImageBinding(m_NormalRoughBinding, renderTargets[VT_RENDER_TARGET_NORMAL_ROUGH]);
		updateImageBinding(m_MetalEmitBinding, renderTargets[VT_RENDER_TARGET_METAL_EMIT]);
		updateImageBinding(m_DepthBinding, renderTargets[VT_RENDER_TARGET_DEPTH]);

		CameraUBO camUBO{};
		// TODO [01.04.26]: Change this
		camUBO.viewProj = m_Camera->GetViewProjection();
		camUBO.invViewProj = m_Camera->GetInverseViewProjection();
		camUBO.cameraPos = m_Camera->GetPosition();

		LightUBO lightUBO;
		int count = 0;

		auto group = m_ActiveScene->m_Registry.group<TransformComponent>(entt::get<LightComponent>);
		for (auto entity : group)
		{
			auto&& [transform, lightComponent] = group.get<TransformComponent, LightComponent>(entity);

			if (count > MAX_LIGHTS)
				VT_CORE_WARN("Light Count in Scene ({0}) exceeding Max Light Count ({1})", count, MAX_LIGHTS);
				break;

			auto& light = lightUBO.Lights[count];
			light.Position = transform.Translation;
			light.Color = lightComponent.Color;
			light.Intensity = lightComponent.Intensity;

			count++;	
		}

		lightUBO.Count = count;

		m_CameraUBOs[frameIndex]->SetData(&camUBO, sizeof(CameraUBO), 0);
		m_LightUBOs[frameIndex]->SetData(&lightUBO, sizeof(LightUBO), 0);

		RHI::VulkanContext::GetSetManager()->WriteBuffer(
			m_Sets[frameIndex],
			m_CameraUBOBinding,
			m_CameraUBOs[frameIndex]->GetVulkanBuffer(),
			sizeof(CameraUBO)
		);

		RHI::VulkanContext::GetSetManager()->WriteBuffer(
			m_Sets[frameIndex],
			m_LightUBOBinding,
			m_LightUBOs[frameIndex]->GetVulkanBuffer(),
			sizeof(LightUBO)
		);
	}

	const VkDescriptorSet& DefferedShaderInput::GetSet() const 
	{ 
		return m_Sets[Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex()]; 
	}

}