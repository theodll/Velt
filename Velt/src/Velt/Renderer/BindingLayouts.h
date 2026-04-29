#pragma once
#include "Core/Core.h"
#include "Platform/Vulkan/DescriptorLayoutCache.h"
#include <vector>

namespace Velt
	// Hi to anyone looking or reviewing this. I know this is not the best thing to do and should probably be replaced with reflection, but I tried it and it didnt work and only did problems so i just did this. forgive me
{
	// Global Set (Set 0) Bindings

	constexpr u32 VT_CAMERA_SET_UBO_BINDING = 0;

	static std::vector<RHI::DescriptorBinding> GetGlobalSetBindings()
	{
		std::vector<RHI::DescriptorBinding> bindings{};

		RHI::DescriptorBinding cameraUBO{};
		cameraUBO.type = RHI::DescriptorType::UNIFORM_BUFFER;
		cameraUBO.binding = VT_CAMERA_SET_UBO_BINDING;
		cameraUBO.count = 1;
		cameraUBO.stage = RHI::ShaderStage::VERTEX;
		bindings.push_back(cameraUBO);

		return bindings;
	}
	

	// Material Set (Set 1) Bindings

	enum MaterialSetBinding : u32
	{
		VT_MATERIAL_SET_BINDING_UBO = 0,
		VT_MATERIAL_SET_BINDING_ALBEDO = 1,
		VT_MATERIAL_SET_BINDING_NORMAL = 2,
		VT_MATERIAL_SET_BINDING_METALLIC = 3,
		VT_MATERIAL_SET_BINDING_ROUGHNESS = 4,
		VT_MATERIAL_SET_BINDING_SAMPLER = 5,
		VT_MATERIAL_SET_BINDING_AMBIENT_OCCLUSION = 10
	};

	static std::vector<RHI::DescriptorBinding> GetMaterialSetBindings()
	{
		std::vector<RHI::DescriptorBinding> bindings{};

		RHI::DescriptorBinding materialUBO{};
		materialUBO.type = RHI::DescriptorType::UNIFORM_BUFFER;
		materialUBO.binding = VT_MATERIAL_SET_BINDING_UBO;
		materialUBO.count = 1;
		materialUBO.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(materialUBO);

		RHI::DescriptorBinding albedoTexture{};
		albedoTexture.type = RHI::DescriptorType::SAMPLED_IMAGE;
		albedoTexture.binding = VT_MATERIAL_SET_BINDING_ALBEDO;
		albedoTexture.count = 1;
		albedoTexture.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(albedoTexture);

		RHI::DescriptorBinding normalTexture{};
		normalTexture.type = RHI::DescriptorType::SAMPLED_IMAGE;
		normalTexture.binding = VT_MATERIAL_SET_BINDING_NORMAL;
		normalTexture.count = 1;
		normalTexture.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(normalTexture);

		RHI::DescriptorBinding roughnessTexture{};
		roughnessTexture.type = RHI::DescriptorType::SAMPLED_IMAGE;
		roughnessTexture.binding = VT_MATERIAL_SET_BINDING_METALLIC;
		roughnessTexture.count = 1;
		roughnessTexture.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(roughnessTexture);

		RHI::DescriptorBinding metallicTexture{};
		metallicTexture.type = RHI::DescriptorType::SAMPLED_IMAGE;
		metallicTexture.binding = VT_MATERIAL_SET_BINDING_ROUGHNESS;
		metallicTexture.count = 1;
		metallicTexture.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(metallicTexture);

		RHI::DescriptorBinding textureSampler{};
		textureSampler.type = RHI::DescriptorType::SAMPLER;
		textureSampler.binding = VT_MATERIAL_SET_BINDING_SAMPLER;
		textureSampler.count = 1;
		textureSampler.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(textureSampler);

		return bindings;
	}
	

	// Deferred Set (Set 2) Bindings

	enum DefferedSetBinding : u32
	{
		VT_DEFFERED_SET_BINDING_ALBEDO_AO = 0,
		VT_DEFFERED_SET_BINDING_NORMAL_ROUGH = 1,
		VT_DEFFERED_SET_BINDING_METAL_EMIT = 2,
		VT_DEFFERED_SET_BINDING_DEPTH = 3,
		VT_DEFFERED_SET_BINDING_SAMPLER = 4,
		VT_DEFFERED_SET_BINDING_CAMERA_UBO = 5,
		VT_DEFFERED_SET_BINDING_LIGHT_UBO = 6,
		VT_DEFFERED_SET_BINDING_COUNT
	};

	static std::vector<RHI::DescriptorBinding> GetDefferedSetBindings()
	{
		std::vector<RHI::DescriptorBinding> bindings{};

		RHI::DescriptorBinding albedoAO{};
		albedoAO.type = RHI::DescriptorType::SAMPLED_IMAGE;
		albedoAO.binding = VT_DEFFERED_SET_BINDING_ALBEDO_AO;
		albedoAO.count = 1;
		albedoAO.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(albedoAO);

		RHI::DescriptorBinding normalRough{};
		normalRough.type = RHI::DescriptorType::SAMPLED_IMAGE;
		normalRough.binding = VT_DEFFERED_SET_BINDING_NORMAL_ROUGH;
		normalRough.count = 1;
		normalRough.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(normalRough);

		RHI::DescriptorBinding metalEmit{};
		metalEmit.type = RHI::DescriptorType::SAMPLED_IMAGE;
		metalEmit.binding = VT_DEFFERED_SET_BINDING_METAL_EMIT;
		metalEmit.count = 1;
		metalEmit.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(metalEmit);

		RHI::DescriptorBinding depth{};
		depth.type = RHI::DescriptorType::SAMPLED_IMAGE;
		depth.binding = VT_DEFFERED_SET_BINDING_DEPTH;
		depth.count = 1;
		depth.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(depth);

		RHI::DescriptorBinding sampler{};
		sampler.type = RHI::DescriptorType::SAMPLER;
		sampler.binding = VT_DEFFERED_SET_BINDING_SAMPLER;
		sampler.count = 1;
		sampler.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(sampler);

		RHI::DescriptorBinding cameraUbo{};
		cameraUbo.type = RHI::DescriptorType::UNIFORM_BUFFER;
		cameraUbo.binding = VT_DEFFERED_SET_BINDING_CAMERA_UBO;
		cameraUbo.count = 1;
		cameraUbo.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(cameraUbo);

		RHI::DescriptorBinding lightUbo{};
		lightUbo.type = RHI::DescriptorType::UNIFORM_BUFFER;
		lightUbo.binding = VT_DEFFERED_SET_BINDING_LIGHT_UBO;
		lightUbo.count = 1;
		lightUbo.stage = RHI::ShaderStage::FRAGMENT;
		bindings.push_back(lightUbo);

		return bindings;
	}
	
}
