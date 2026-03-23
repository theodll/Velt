#include "ShaderLibrary.h"
#include "Core/Core.h"
#include "spirv_reflect.h"
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
		auto reflectData = ReflectShader(bytecode, stage);

		auto newShader = CreateRef<Shader>();
		newShader->Module = module;
		newShader->Stage = stage;
		newShader->ReflectData = reflectData;

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

	static RHI::DescriptorType FromSpvReflectDescriptorType(SpvReflectDescriptorType type)
	{
		switch (type)
		{
		case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLER: return RHI::DescriptorType::SAMPLER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return RHI::DescriptorType::COMBINED_IMAGE_SAMPLER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE: return RHI::DescriptorType::SAMPLED_IMAGE;
		case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE: return RHI::DescriptorType::STORAGE_IMAGE;
		case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: return RHI::DescriptorType::UNIFORM_TEXEL_BUFFER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: return RHI::DescriptorType::STORAGE_TEXEL_BUFFER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return RHI::DescriptorType::UNIFORM_BUFFER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER: return RHI::DescriptorType::STORAGE_BUFFER;
		case SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: return RHI::DescriptorType::UNIFORM_BUFFER_DYNAMIC;
		case SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return RHI::DescriptorType::STORAGE_BUFFER_DYNAMIC;
		case SPV_REFLECT_DESCRIPTOR_TYPE_INPUT_ATTACHMENT: return RHI::DescriptorType::INPUT_ATTACHMENT;
		default:
			VT_CORE_ASSERT(false, "Unsupported descriptor type from SPIRV-Reflect");
			return RHI::DescriptorType{};
		}
	}

	static RHI::ShaderStage FromSpvShaderStage(SpvReflectShaderStageFlagBits stage)
	{
		switch (stage)
		{
		case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT: return RHI::ShaderStage::VERTEX;
		case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT: return RHI::ShaderStage::FRAGMENT;
		default:
			VT_CORE_ASSERT(false, "Unsupported Stage Flag from SPIRV-Reflect");
			return RHI::ShaderStage{};
		}
	}

	std::unordered_map<u32, DescriptorSetLayoutData> ShaderLibrary::ReflectShader(const std::vector<u32>& bytes, RHI::ShaderStage& outStage)
	{
		SpvReflectShaderModule module{};
		SpvReflectResult result = spvReflectCreateShaderModule(bytes.size() * sizeof(u32), bytes.data(), &module);
		VT_CORE_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS, "Failed to create SpvReflectShaderModule");

		outStage = FromSpvShaderStage(module.shader_stage);

		u32 count{};
		result = spvReflectEnumerateDescriptorSets(&module, &count, VT_NULL_HANDLE);
		VT_CORE_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS, "Failed to enumerate Descriptor Sets in Shader Module");

		std::vector<SpvReflectDescriptorSet*> sets(count);
		result = spvReflectEnumerateDescriptorSets(&module, &count, sets.data());
		VT_CORE_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS, "Failed to enumerate Descriptor Sets in Shader Module");

		std::unordered_map<u32, DescriptorSetLayoutData> set_layouts;

		for (size_t i_set = 0; i_set < sets.size(); ++i_set) {
			const SpvReflectDescriptorSet& reflectionSet = *(sets[i_set]);

			DescriptorSetLayoutData layout{};
			layout.SetNumber = reflectionSet.set;
			layout.Bindings.resize(reflectionSet.binding_count);

			for (uint32_t i_binding = 0; i_binding < reflectionSet.binding_count; ++i_binding) {
				const SpvReflectDescriptorBinding& refl_binding = *(reflectionSet.bindings[i_binding]);
				RHI::DescriptorBinding& layout_binding = layout.Bindings[i_binding];

				layout_binding.binding = refl_binding.binding;
				layout_binding.type = FromSpvReflectDescriptorType(refl_binding.descriptor_type);
				layout_binding.count = 1;
				layout_binding.name = refl_binding.name ? refl_binding.name : "";

				for (uint32_t i_dim = 0; i_dim < refl_binding.array.dims_count; ++i_dim) {
					layout_binding.count *= refl_binding.array.dims[i_dim];
				}

				layout_binding.stage = outStage;
			}

			set_layouts[layout.SetNumber] = layout;
		}

		spvReflectDestroyShaderModule(&module);

		return set_layouts;
	}
}