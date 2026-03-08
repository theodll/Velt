#include "ShaderLibrary.h"
#include "Core/Core.h"

namespace Velt 
{
	std::unordered_map <std::filesystem::path, Ref<Shader >> ShaderLibrary::m_Shaders{};
	Ref<Shader> ShaderLibrary::Get(const std::filesystem::path& path)
	{
		auto it = m_Shaders.find(path);
		if (it != m_Shaders.end()) 
		{
			return it->second;
		}

		std::vector<u32> bytecode = ReadSpirvFile(path);

		VkShaderModule module = CreateVulkanModule(bytecode);
	}

	std::vector<u32> ShaderLibrary::ReadSpirvFile(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		VT_CORE_ASSERT(file, "Failed to open file: {}", path.string());

		size_t fileSize = static_cast<size_t>(file.tellg());
		VT_CORE_ASSERT(fileSize % 4 == 0, "Invalid SPIR-V size");

		std::vector<u32> buffer(fileSize / sizeof(u32));

		file.seekg(0);
		file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

		return buffer;
	}

	VkShaderModule ShaderLibrary::CreateVulkanModule(const std::vector<u32>& bytes)
	{
		return VT_NULL_HANDLE;
	}

	DescriptorSetLayoutData ReflectShader(const std::vector<u32>& bytes)
	{
		return {};
	}
}