#pragma once
#include "Core/Core.h"
#include "Core/Math.h"
#include "Texture.h"
#include "Pipeline.h"
#include "Camera.h"
#include <vulkan/vulkan.h>

namespace Velt
{
	struct GBuffer 
	{
		std::unordered_map<u32, Ref<Texture2D>> Targets;

		Ref<Texture2D> operator[](int i) 
		{
			return Targets[i];
		}
	};

	class DefferedShaderInput;

	class VELT_API DefferedRenderer 
	{
	public:
		void Init(Ref<Camera> pCamera);
		void Shutdown();

		void ExecuteDefferedPass(VkCommandBuffer cmd);

		static Ref<Pipeline> GetPipeline() { return s_DefferedPipeline; }
	private:
		static Ref<Pipeline> s_DefferedPipeline;
	

		Ref<DefferedShaderInput> m_ShaderInput;
	};

	class DefferedShaderInput
	{
	public:
		DefferedShaderInput(Ref<Camera> pCamera);
		virtual ~DefferedShaderInput() 
		{
			m_TextureSampler.reset();
			
			for (auto& gbuffer : m_GeometryBuffers)
			{
				gbuffer.reset();
			}
		};

		const VkDescriptorSet& GetSet() const; 
		void UpdateData();

	private:
		
		Ref<Camera> m_Camera;

		VkDescriptorSet m_Sets[MAX_FRAMES_IN_FLIGHT];
		Ref<GBuffer> m_GeometryBuffers[MAX_FRAMES_IN_FLIGHT];

		Ref<Texture2D> m_TextureSampler;


		struct alignas(16) CameraUBO
		{
			Matrix viewProj;
			Matrix invViewProj;
			Vector cameraPos;
		};

		std::vector<Ref<UniformBuffer>> m_CameraUBOs;
		u32 m_UBOBinding{};

		u32 m_AlbedoAOBinding{};
		u32 m_NormalRoughBinding{};
		u32 m_MetalEmitBinding{};
		u32 m_DepthBinding{};
		u32 m_CompositeBinding{};
		u32 m_SamplerBinding{};
		std::unordered_set<u32> m_ValidBindings;
	};
}