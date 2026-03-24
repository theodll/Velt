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

			PipelineSpecification defferedPBRPipelineSpecification{};
			defferedPBRPipelineSpecification.VertexShader = dPBRVertexShader;
			defferedPBRPipelineSpecification.FragmentShader = dPBRPixelShader;
			defferedPBRPipelineSpecification.Layout = defferedLayout;

			s_DefferedPipeline = Pipeline::Create(&defferedPBRPipelineSpecification);
			s_DefferedPipeline->Init();
		}
		m_ShaderInput = CreateRef<DefferedShaderInput>();
	}

	void DefferedRenderer::Shutdown()
	{

	}

	void DefferedRenderer::ExecuteDefferedPass()
	{
		auto cmd = Application::Get()->GetWindow()->GetSwapchain()->GetCurrentDrawCommandBuffer();
		m_ShaderInput->UpdateData();
		Renderer::SubmitFullscreenTriangle(cmd, s_DefferedPipeline, m_ShaderInput);
	}

	DefferedShaderInput::DefferedShaderInput()
	{
		auto pipeline = DefferedRenderer::GetPipeline();
		VT_CORE_ASSERT(pipeline, "");
		const auto& spec = pipeline->GetSpecification();
		VT_CORE_ASSERT(spec.FragmentShader, "");
		auto setIt = spec.FragmentShader->ReflectData.find(1);
		VT_CORE_ASSERT(setIt != spec.FragmentShader->ReflectData.end(), "");
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
		
		requireBinding(m_AlbedoAOBinding, RHI::DescriptorType::SAMPLED_IMAGE);
		requireBinding(m_NormalRoughBinding, RHI::DescriptorType::SAMPLED_IMAGE);
		requireBinding(m_MetalEmitBinding, RHI::DescriptorType::SAMPLED_IMAGE);
		requireBinding(m_SamplerBinding, RHI::DescriptorType::SAMPLER);

		auto hasBinding = [&](u32 binding) -> bool
			{
				return m_ValidBindings.find(binding) != m_ValidBindings.end();
			};

		for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_GeometryBuffers[i] = CreateRef<GBuffer>();

			const auto& layouts = pipeline->GetSetLayouts();
			VT_CORE_ASSERT(layouts.size() > 1, "");

			// Todo [27.02, Theo] Maybe change this that this isn't a magic number but still, I couldn't 
			// care less

			// Note [27.02, Theo] The one stands for the Material Set Layout (set 0 = global, set 1 = material)
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
	}

	const VkDescriptorSet& DefferedShaderInput::GetSet() const 
	{ 
		return m_Sets[Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex()]; 
	}

}