#pragma once
#include "Core/Core.h"
#include "Texture.h"
#include "Pipeline.h"
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
		void Init();
		void Shutdown();

		void ExecuteDefferedPass();

		static Ref<Pipeline> GetPipeline() { return s_DefferedPipeline; }
	private:
		static Ref<Pipeline> s_DefferedPipeline;

		u32 m_CameraUBOBinding;

		Ref<DefferedShaderInput> m_ShaderInput;
	};

	class DefferedShaderInput
	{
	public:
		DefferedShaderInput();
		virtual ~DefferedShaderInput() = default;

		const VkDescriptorSet& GetSet() const; 
		void UpdateData();

	private:
		
		VkDescriptorSet m_Sets[MAX_FRAMES_IN_FLIGHT];
		Ref<GBuffer> m_GeometryBuffers[MAX_FRAMES_IN_FLIGHT];

		Ref<Texture2D> s_TextureSampler;

		u32 m_AlbedoAOBinding{};
		u32 m_NormalRoughBinding{};
		u32 m_MetalEmitBinding{};
		u32 m_DepthBinding{};
		u32 m_CompositeBinding{};
		u32 m_SamplerBinding{};
		std::unordered_set<u32> m_ValidBindings;
	};
}