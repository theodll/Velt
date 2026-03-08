#pragma once
#include "Core/Core.h"
#include <vulkan/vulkan.h>
#include "Platform/Vulkan/DescriptorLayoutCache.h"
#include <vector>
#include <unordered_map>
#include <filesystem>

// Todo [08.03.26, Theo] Make platform independent


namespace Velt
{
	struct DescriptorSetLayoutData 
	{
		u32 SetNumber;
		std::vector<RHI::DescriptorBinding> Bindings;
	};

	struct Shader 
	{
		VkShaderModule Module{};
		RHI::ShaderStage Stage{};

		std::vector<DescriptorSetLayoutData> ReflectData{}; 
		std::vector<VkPushConstantRange> PushConstats{};
	};

	class ShaderLibrary 
	{
	public:
		static Ref<Shader> Get(const std::filesystem::path& path);

	private: 
		static std::vector<u32> ReadSpirvFile(const std::filesystem::path& path);
		static VkShaderModule CreateVulkanModule(const std::vector<u32>& bytes);
		static DescriptorSetLayoutData ReflectShader(const std::vector<u32>& bytes);

		static std::unordered_map<std::filesystem::path, Ref<Shader>> m_Shaders;


	};
}