#pragma once
#include "Core/Core.h"
#include <vulkan/vulkan.h>
#include "Platform/Vulkan/DescriptorLayoutCache.h"
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <string>

namespace Velt
{
	struct Shader
	{
		VkShaderModule Module{};
		RHI::ShaderStage Stage{};

		// Todo [09.03.26, Theo]: make push constants
		// std::vector<VkPushConstantRange> PushConstants{};
	};

	class ShaderLibrary
	{
	public:
		static Ref<Shader> Get(const std::filesystem::path& path);

	private:
		static std::vector<u32> ReadSpirvFile(const std::filesystem::path& path);
		static VkShaderModule CreateVulkanModule(const std::vector<u32>& bytes);

		static std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}