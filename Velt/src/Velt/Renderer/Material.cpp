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
			for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				m_UBOs[i] = UniformBuffer::Create(sizeof(MaterialUBO));

				auto& layouts = SceneRenderer::GetPipeline()->GetSetLayouts();

				// Todo [27.02, Theo] Maybe change this that this isn't a magic number but still, I couldn't 
				// care less

				// Note [27.02, Theo] The one stands for the Material Set Layout (set 0 = global, set 1 = material)
				m_Sets[i] = RHI::VulkanContext::GetSetManager()->Allocate(layouts[1]);

				RHI::VulkanContext::GetSetManager()->WriteBuffer(
					m_Sets[i],
					0,
					m_UBOs[i]->GetVulkanBuffer(),
					sizeof(MaterialUBO)
				);
			}

			m_Sampler = Texture2D::Create(ERROR_TEXTURE_PATH);
			for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				RHI::VulkanContext::GetSetManager()->WriteSampler(
					m_Sets[i],
					VT_MATERIAL_SLOTS_BINDING_SAMPLER,
					m_Sampler->GetSampler()
				);
			}

			const auto defaultTexture = GetDefaultMaterialTexture();
			SetAlbedoTexture(defaultTexture);
			SetNormalTexture(defaultTexture);
			SetMetalllicTexture(defaultTexture);
			SetRoughnessTexture(defaultTexture);

			UpdateData();
		};

		void Material::SetTexture(u32 binding, Ref<Texture2D> pTexture)
		{
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