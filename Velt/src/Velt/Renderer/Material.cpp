#include "Core/Core.h"
#include "Material.h"
#include "UniformBuffer.h"
#include "SceneRenderer.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/DescriptorSetManager.h"
#include "Core/Application.h"

namespace Velt
{
		namespace
		{
			Ref<Texture2D> GetDefaultMaterialTexture()
			{
				static Ref<Texture2D> s_DefaultTexture = Texture2D::Create(ERROR_TEXTURE_PATH);
				return s_DefaultTexture;
			}
		}

		Material::Material()
		{
			auto pipeline = SceneRenderer::GetPipeline();
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

			m_MaterialUBOBinding = VT_MATERIAL_SLOTS_BINDING_UBO;
			m_AlbedoBinding = VT_MATERIAL_SLOTS_BINDING_ALBEDO;
			m_NormalBinding = VT_MATERIAL_SLOTS_BINDING_NORMAL;
			m_MetallicBinding = VT_MATERIAL_SLOTS_BINDING_METALLIC;
			m_RoughnessBinding = VT_MATERIAL_SLOTS_BINDING_ROUGHNESS;
			m_SamplerBinding = VT_MATERIAL_SLOTS_BINDING_SAMPLER;

			requireBinding(m_MaterialUBOBinding, RHI::DescriptorType::UNIFORM_BUFFER);
			requireBinding(m_AlbedoBinding, RHI::DescriptorType::SAMPLED_IMAGE);
			requireBinding(m_NormalBinding, RHI::DescriptorType::SAMPLED_IMAGE);
			requireBinding(m_MetallicBinding, RHI::DescriptorType::SAMPLED_IMAGE);
			optionalBinding(m_RoughnessBinding, RHI::DescriptorType::SAMPLED_IMAGE);
			requireBinding(m_SamplerBinding, RHI::DescriptorType::SAMPLER);

			auto hasBinding = [&](u32 binding) -> bool
			{
				return m_ValidBindings.find(binding) != m_ValidBindings.end();
			};

			for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				m_UBOs[i] = UniformBuffer::Create(sizeof(MaterialUBO));

				const auto& layouts = pipeline->GetSetLayouts();
				VT_CORE_ASSERT(layouts.size() > 1, "");

				// Todo [27.02, Theo] Maybe change this that this isn't a magic number but still, I couldn't 
				// care less

				// Note [27.02, Theo] The one stands for the Material Set Layout (set 0 = global, set 1 = material)
				m_Sets[i] = RHI::VulkanContext::GetSetManager()->Allocate(layouts[1]);

				if (hasBinding(m_MaterialUBOBinding))
				{
					RHI::VulkanContext::GetSetManager()->WriteBuffer(
						m_Sets[i],
						m_MaterialUBOBinding,
						m_UBOs[i]->GetVulkanBuffer(),
						sizeof(MaterialUBO)
					);
				}
			}

			m_Sampler = Texture2D::Create(ERROR_TEXTURE_PATH);
			for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				if (hasBinding(m_SamplerBinding))	
				{
					RHI::VulkanContext::GetSetManager()->WriteSampler(
						m_Sets[i],
						m_SamplerBinding,
						m_Sampler->GetSampler()
					);
				}
			}

			const auto defaultTexture = GetDefaultMaterialTexture();
			SetAlbedoTexture(defaultTexture);
			SetNormalTexture(defaultTexture);
			SetMetalllicTexture(defaultTexture);
			if (m_ValidBindings.find(m_RoughnessBinding) != m_ValidBindings.end())
				SetRoughnessTexture(defaultTexture);

			UpdateData();
		};

		void Material::SetTexture(u32 binding, Ref<Texture2D> pTexture)
		{
			if (m_ValidBindings.find(binding) == m_ValidBindings.end())
				return;
			m_Textures[binding] = pTexture;

			for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
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
		}

		std::vector<Velt::RHI::DescriptorBinding> Material::GetMaterialBindings()
		{
			std::vector<Velt::RHI::DescriptorBinding> materialBindings{};

			RHI::DescriptorBinding uninformData{};
			uninformData.type = RHI::DescriptorType::UNIFORM_BUFFER;
			uninformData.binding = VT_MATERIAL_SLOTS_BINDING_UBO;
			uninformData.count = 1;
			uninformData.stage = RHI::ShaderStage::FRAGMENT;
			materialBindings.push_back(uninformData);

			RHI::DescriptorBinding albedoTexture{};
			albedoTexture.type = RHI::DescriptorType::SAMPLED_IMAGE;
			albedoTexture.binding = VT_MATERIAL_SLOTS_BINDING_ALBEDO;
			albedoTexture.count = 1;
			albedoTexture.stage = RHI::ShaderStage::FRAGMENT;
			materialBindings.push_back(albedoTexture);

			RHI::DescriptorBinding normalTexture{};
			normalTexture.type = RHI::DescriptorType::SAMPLED_IMAGE;
			normalTexture.binding = VT_MATERIAL_SLOTS_BINDING_NORMAL;
			normalTexture.count = 1;
			normalTexture.stage = RHI::ShaderStage::FRAGMENT;
			materialBindings.push_back(normalTexture);

			RHI::DescriptorBinding roughnessTexture{};
			roughnessTexture.type = RHI::DescriptorType::SAMPLED_IMAGE;
			roughnessTexture.binding = VT_MATERIAL_SLOTS_BINDING_ROUGHNESS;
			roughnessTexture.count = 1;
			roughnessTexture.stage = RHI::ShaderStage::FRAGMENT;
			materialBindings.push_back(roughnessTexture);

			RHI::DescriptorBinding metallicTexture{};
			metallicTexture.type = RHI::DescriptorType::SAMPLED_IMAGE;
			metallicTexture.binding = VT_MATERIAL_SLOTS_BINDING_METALLIC;
			metallicTexture.count = 1;
			metallicTexture.stage = RHI::ShaderStage::FRAGMENT;
			materialBindings.push_back(metallicTexture);

			/*
			RHI::DescriptorBinding aoTexture{};
			aoTexture.type = RHI::DescriptorType::SAMPLED_IMAGE;
			aoTexture.binding = VT_MATERIAL_SLOTS_BINDING_AMBIENT_OCCLUSION;
			aoTexture.count = 1;
			aoTexture.stage = RHI::ShaderStage::FRAGMENT;
			materialBindings.push_back(aoTexture); */

			RHI::DescriptorBinding textureSampler{};
			textureSampler.type = RHI::DescriptorType::SAMPLER;
			textureSampler.binding = VT_MATERIAL_SLOTS_BINDING_SAMPLER;
			textureSampler.count = 1;
			textureSampler.stage = RHI::ShaderStage::FRAGMENT;
			materialBindings.push_back(textureSampler);
			
			return materialBindings;
		}

		const VkDescriptorSet& Material::GetSet() const { return m_Sets[Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex()]; }

		void Material::UpdateData()
		{
			for (int i{ 0 }; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				m_UBOs[i]->SetData(&m_Data, sizeof(MaterialUBO), 0);
			}

		}

		MaterialTable::MaterialTable(uint32_t materialCount)
			: m_MaterialCount(materialCount)
		{
		}

		MaterialTable::MaterialTable(Ref<MaterialTable> other)
			: m_MaterialCount(other->m_MaterialCount)
		{
			const auto& meshMaterials = other->GetMaterials();
			for (auto [index, materialAsset] : meshMaterials)
				SetMaterial(index, materialAsset);
		}

		void MaterialTable::SetMaterial(uint32_t index, Ref<Material> material)
		{
			m_Materials[index] = material;
			if (index >= m_MaterialCount)
				m_MaterialCount = index + 1;
		}

		void MaterialTable::ClearMaterial(uint32_t index)
		{
			VT_CORE_ASSERT(HasMaterial(index), "");
			m_Materials.erase(index);
			if (index >= m_MaterialCount)
				m_MaterialCount = index + 1;
		}

		void MaterialTable::Clear()
		{
			m_Materials.clear();
		}
}