#include "ShaderLibrary.h"
#include "Core/Core.h"
#include <fstream> 

namespace Velt
{
	std::unordered_map<std::string, Ref<Shader>> ShaderLibrary::m_Shaders{};

	Ref<Shader> ShaderLibrary::Get(const std::filesystem::path& path)
	{
		std::string pathStr = path.string();

		auto it = m_Shaders.find(pathStr);
		if (it != m_Shaders.end())
		{
			return it->second;
		}

		std::vector<u32> bytecode = ReadSpirvFile(path);
		VkShaderModule module = CreateVulkanModule(bytecode);

		RHI::ShaderStage stage{};

		auto newShader = CreateRef<Shader>();
		newShader->Module = module;
		newShader->Stage = stage;

		m_Shaders[pathStr] = newShader;
		return newShader;
	}

	std::vector<u32> ShaderLibrary::ReadSpirvFile(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		VT_CORE_ASSERT(file.is_open(), "Failed to open file: {}", path.string());

		size_t fileSize = static_cast<size_t>(file.tellg());
		VT_CORE_ASSERT(fileSize % 4 == 0, "Invalid SPIR-V size");

		std::vector<u32> buffer(fileSize / sizeof(u32));

		file.seekg(0);
		file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

		return buffer;
	}

	VkShaderModule ShaderLibrary::CreateVulkanModule(const std::vector<u32>& bytes)
	{
		VkShaderModule module;

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pNext = VT_NULL_HANDLE;
		createInfo.flags = 0;
		createInfo.codeSize = bytes.size() * sizeof(u32);
		createInfo.pCode = bytes.data();

		VT_VK_CHECK(
			vkCreateShaderModule(RHI::VulkanContext::GetDevice()->device(), &createInfo, VT_NULL_HANDLE, &module),
			"Failed to create shader module!"
		);

		return module;
	}
}