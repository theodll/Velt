#include "vtpch.h"
#include "DefferedRenderer.h"
#include "Renderer.h"
#include "Core/Application.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/DescriptorSetManager.h"
#include "BindingLayouts.h"

namespace Velt 
{
	Ref<Pipeline> DefferedRenderer::s_DefferedPipeline = nullptr;

	void DefferedRenderer::Init()
	{
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

			m_CameraUBOBinding = VT_CAMERA_SET_UBO_BINDING;
		}

		m_CameraUBOs.resize(MAX_FRAMES_IN_FLIGHT);
		m_GlobalSets.resize(MAX_FRAMES_IN_FLIGHT);

		const std::vector<VkDescriptorSetLayout> setLayouts = s_DefferedPipeline->GetSetLayouts();
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

		m_ShaderInput = CreateRef<DefferedShaderInput>();


	}

	void DefferedRenderer::Shutdown()
	{

	}

	void DefferedRenderer::ExecuteDefferedPass()
	{
		auto cmd = Application::Get()->GetWindow()->GetSwapchain()->GetCurrentDrawCommandBuffer();

		CameraUBO ubo{};
		ubo.viewProj = SceneRenderer::GetCamera()->GetViewProjection();
		ubo.invViewProj = SceneRenderer::GetCamera()->GetInverseViewProjection();

		auto frameIndex = Velt::Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex();
		m_CameraUBOs[frameIndex]->SetData(&ubo, sizeof(CameraUBO), 0);

		// Todo [25.02, Theo]: Make this platform independent
		vkCmdBindDescriptorSets(
			cmd,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			s_DefferedPipeline->GetVulkanPipelineLayout(),
			0,
			1,
			&m_GlobalSets[frameIndex],
			0,
			nullptr
		);

		m_ShaderInput->UpdateData();
		Renderer::SubmitFullscreenTriangle(cmd, s_DefferedPipeline, m_ShaderInput);
	}

	DefferedShaderInput::DefferedShaderInput()
	{
		auto pipeline = DefferedRenderer::GetPipeline();
		VT_CORE_ASSERT(pipeline, "");
		
		m_AlbedoAOBinding = VT_DEFFERED_SET_BINDING_ALBEDO_AO;
		m_NormalRoughBinding = VT_DEFFERED_SET_BINDING_NORMAL_ROUGH;
		m_MetalEmitBinding = VT_DEFFERED_SET_BINDING_METAL_EMIT;
		m_DepthBinding = VT_DEFFERED_SET_BINDING_DEPTH;
		m_SamplerBinding = VT_DEFFERED_SET_BINDING_SAMPLER;
		
		m_ValidBindings.emplace(m_AlbedoAOBinding);
		m_ValidBindings.emplace(m_NormalRoughBinding);
		m_ValidBindings.emplace(m_MetalEmitBinding);
		m_ValidBindings.emplace(m_DepthBinding);
		m_ValidBindings.emplace(m_SamplerBinding);

		for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_GeometryBuffers[i] = CreateRef<GBuffer>();

			const auto& layouts = pipeline->GetSetLayouts();
			VT_CORE_ASSERT(layouts.size() > 2, "");

			// Note [27.02, Theo] The two stands for the Deferred Set Layout (set 0 = global, set 1 = material, set 2 = deferred)
			m_Sets[i] = RHI::VulkanContext::GetSetManager()->Allocate(layouts[2]);

		}

		s_TextureSampler = Texture2D::Create(ERROR_TEXTURE_PATH);
		for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			RHI::VulkanContext::GetSetManager()->WriteSampler(
				m_Sets[i],
				m_SamplerBinding,
				s_TextureSampler->GetSampler()
			);
		}

		UpdateData();
	}

	void DefferedShaderInput::UpdateData()
	{
		auto renderTargets = Renderer::GetRenderTargets();

		auto updateImageBinding = [&](u32 binding, Ref<Texture2D> pTexture)
		{
			if (m_ValidBindings.find(binding) == m_ValidBindings.end() || !pTexture)
				return;

			for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				m_GeometryBuffers[i]->Targets[binding] = pTexture;
				
				VkDescriptorImageInfo info{};
				info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				info.imageView = pTexture->GetImageView();
				info.sampler = VT_NULL_HANDLE;

				RHI::VulkanContext::GetSetManager()->WriteImage(
					m_Sets[i],
					binding,
					info
				);
			}
		};

		updateImageBinding(m_AlbedoAOBinding, renderTargets[VT_RENDER_TARGET_ALBEDO_AO]);
		updateImageBinding(m_NormalRoughBinding, renderTargets[VT_RENDER_TARGET_NORMAL_ROUGH]);
		updateImageBinding(m_MetalEmitBinding, renderTargets[VT_RENDER_TARGET_METAL_EMIT]);
		updateImageBinding(m_DepthBinding, renderTargets[VT_RENDER_TARGET_DEPTH]);
	}

	const VkDescriptorSet& DefferedShaderInput::GetSet() const 
	{ 
		return m_Sets[Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex()]; 
	}

}