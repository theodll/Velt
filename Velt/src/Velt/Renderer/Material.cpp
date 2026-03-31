#include "Core/Core.h"
#include "Material.h"
#include "UniformBuffer.h"
#include "SceneRenderer.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/DescriptorSetManager.h"
#include "Core/Application.h"
#include "BindingLayouts.h"

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
			VT_PROFILE_FUNCTION();
			m_ValidBindings.insert(VT_MATERIAL_SET_BINDING_UBO);
			m_ValidBindings.insert(VT_MATERIAL_SET_BINDING_ALBEDO);
			m_ValidBindings.insert(VT_MATERIAL_SET_BINDING_NORMAL);
			m_ValidBindings.insert(VT_MATERIAL_SET_BINDING_METALLIC);
			m_ValidBindings.insert(VT_MATERIAL_SET_BINDING_ROUGHNESS);
			m_ValidBindings.insert(VT_MATERIAL_SET_BINDING_SAMPLER);

			for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				m_UBOs[i] = UniformBuffer::Create(sizeof(MaterialUBO));

				auto& layouts = SceneRenderer::GetPipeline()->GetSetLayouts();

				// Note [27.02, Theo] The one stands for the Material Set Layout (set 0 = global, set 1 = material)
				m_Sets[i] = RHI::VulkanContext::GetSetManager()->Allocate(layouts[1]);

				RHI::VulkanContext::GetSetManager()->WriteBuffer(
					m_Sets[i],
					VT_MATERIAL_SET_BINDING_UBO, 
					m_UBOs[i]->GetVulkanBuffer(),
					sizeof(MaterialUBO)
				);
			}

			m_Sampler = Texture2D::Create(ERROR_TEXTURE_PATH);
			for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				RHI::VulkanContext::GetSetManager()->WriteSampler(
					m_Sets[i],
					VT_MATERIAL_SET_BINDING_SAMPLER,
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
			VT_PROFILE_FUNCTION();
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
			return GetMaterialSetBindings();
		}

		const VkDescriptorSet& Material::GetSet() const { return m_Sets[Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex()]; }

		void Material::UpdateData()
		{
			VT_PROFILE_FUNCTION();
			for (int i{ 0 }; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				m_UBOs[i]->SetData(&m_Data, sizeof(MaterialUBO), 0);
			}

		}

		MaterialTable::MaterialTable(uint32_t materialCount)
			: m_MaterialCount(materialCount)
		{
			VT_PROFILE_FUNCTION();
		}

		MaterialTable::MaterialTable(Ref<MaterialTable> other)
			: m_MaterialCount(other->m_MaterialCount)
		{
			VT_PROFILE_FUNCTION();
			const auto& meshMaterials = other->GetMaterials();
			for (auto [index, materialAsset] : meshMaterials)
				SetMaterial(index, materialAsset);
		}

		void MaterialTable::SetMaterial(uint32_t index, Ref<Material> material)
		{
			VT_PROFILE_FUNCTION();
			m_Materials[index] = material;
			if (index >= m_MaterialCount)
				m_MaterialCount = index + 1;
		}

		void MaterialTable::ClearMaterial(uint32_t index)
		{
			VT_PROFILE_FUNCTION();
			VT_CORE_ASSERT(HasMaterial(index), "");
			m_Materials.erase(index);
			if (index >= m_MaterialCount)
				m_MaterialCount = index + 1;
		}

		void MaterialTable::Clear()
		{
			VT_PROFILE_FUNCTION();
			m_Materials.clear();
		}
}