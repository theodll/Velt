#include "vtpch.h"
#include "DefferedRenderer.h"
#include "Renderer.h"
#include "Core/Application.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/DescriptorSetManager.h"

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

			s_DefferedPipeline = Pipeline::Create(&defferedPBRPipelineSpecification);
			s_DefferedPipeline->Init();

			m_CameraUBOBinding = 0;
			bool foundCameraUBO = false;
			auto setIt = dPBRPixelShader->ReflectData.find(0);
			if (setIt != dPBRPixelShader->ReflectData.end())
			{
				for (const auto& b : setIt->second.Bindings)
				{
					if (b.type == RHI::DescriptorType::UNIFORM_BUFFER)
					{
						m_CameraUBOBinding = b.binding;
						foundCameraUBO = true;
						break;
					}
				}
			}

			VT_CORE_ASSERT(foundCameraUBO, "Did not find Camera UBO Set in Shader Reflection Data");
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
		// Documentation [26.03.26, Theo]: I think this is really confusing so I'm going to explain it.
		// At first we get the Deffered Pipeline (the Pipeline that includes the Deffered Pass Shaders like deffered_pbr_pixel.hlsl.spírv).
		auto pipeline = DefferedRenderer::GetPipeline();
		// Assert if the Pipeline is invalid.
		VT_CORE_ASSERT(pipeline, "");
		// Get the Pipeline  Specification.
		const auto& spec = pipeline->GetSpecification();
		// Assert if the Pipeline Specification is invalid.
		VT_CORE_ASSERT(spec.FragmentShader, "");
		// Now heres the confusing part. We get the Shader Reflection Data of the Pixel Shader, then we search if the Reflection Data includes any "References" 
		// to Descriptor Set 2. Thats what the 2 in ReflectData.find() stands for.
		auto setIt = spec.FragmentShader->ReflectData.find(2);
		// Here we check if the Shader Included any "References" for Set 2. If not return because there's nothing we can upload.
		if(setIt == spec.FragmentShader->ReflectData.end())
		{ 
			return;
		}
		// From now on everybody should understand so I'm not going to explain from here.

		for (const auto& b : setIt->second.Bindings) 
			m_ValidBindings.emplace(b.binding);

		std::unordered_map<u32, RHI::DescriptorBinding> bindingMap;
		bindingMap.reserve(setIt->second.Bindings.size());
		for (const auto& b : setIt->second.Bindings)
			bindingMap[b.binding] = b;

		auto requireBinding = [&](u32 binding, RHI::DescriptorType type)
			{
				auto it = bindingMap.find(binding);
				VT_CORE_ASSERT(it != bindingMap.end(), "");
				VT_CORE_ASSERT(it->second.type == type, "");
			};

		auto optionalBinding = [&](u32 binding, RHI::DescriptorType type) -> bool
			{
				auto it = bindingMap.find(binding);
				if (it == bindingMap.end())
					return false;
				VT_CORE_ASSERT(it->second.type == type, "");
				return true;
			};

		m_AlbedoAOBinding = VT_RENDER_TARGET_ALBEDO_AO;
		m_NormalRoughBinding = VT_RENDER_TARGET_NORMAL_ROUGH;
		m_MetalEmitBinding = VT_RENDER_TARGET_METAL_EMIT;
		m_DepthBinding = VT_RENDER_TARGET_DEPTH;
		m_CompositeBinding = VT_RENDER_TARGET_COMPOSITE;
		m_SamplerBinding = VT_RENDER_TARGET_SAMPLER;
		
		optionalBinding(m_AlbedoAOBinding, RHI::DescriptorType::SAMPLED_IMAGE);
		optionalBinding(m_NormalRoughBinding, RHI::DescriptorType::SAMPLED_IMAGE);
		optionalBinding(m_MetalEmitBinding, RHI::DescriptorType::SAMPLED_IMAGE);
		requireBinding(m_DepthBinding, RHI::DescriptorType::SAMPLED_IMAGE);
		requireBinding(m_SamplerBinding, RHI::DescriptorType::SAMPLER);

		auto hasBinding = [&](u32 binding) -> bool
			{
				return m_ValidBindings.find(binding) != m_ValidBindings.end();
			};

		for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_GeometryBuffers[i] = CreateRef<GBuffer>();

			const auto& layouts = pipeline->GetSetLayouts();
			VT_CORE_ASSERT(layouts.size() > 2, "");

			// Todo [27.02, Theo] Maybe change this that this isn't a magic number but still, I couldn't 
			// care less

			// Note [27.02, Theo] The one stands for the Material Set Layout (set 0 = global, set 1 = material, set 2 = deffered)
			m_Sets[i] = RHI::VulkanContext::GetSetManager()->Allocate(layouts[2]);

		}

		s_TextureSampler = Texture2D::Create(ERROR_TEXTURE_PATH);
		for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (hasBinding(m_SamplerBinding))
			{
				RHI::VulkanContext::GetSetManager()->WriteSampler(
					m_Sets[i],
					m_SamplerBinding,
					s_TextureSampler->GetSampler()
				);
			}
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