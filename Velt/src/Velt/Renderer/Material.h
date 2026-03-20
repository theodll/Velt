#pragma once
#include "Core/Core.h"
#include "Core/Math.h"
#include "Platform/Vulkan/DescriptorLayoutCache.h"
#include "Texture.h"
#include <map>

namespace Velt 
{
	class UniformBuffer;
	class Application;
	


	enum MaterialSlotS : u8
	{
		VT_MATERIAL_SLOTS_BINDING_UBO = 0,
		VT_MATERIAL_SLOTS_BINDING_ALBEDO = 1,
		VT_MATERIAL_SLOTS_BINDING_NORMAL = 2,
		VT_MATERIAL_SLOTS_BINDING_ROUGHNESS = 3,
		VT_MATERIAL_SLOTS_BINDING_METALLIC = 4
	};

	class VELT_API Material
	{
	public:
		Material();
		// Note [18.03.26, Theo]: This probably isn't ideal because of so many write operations

		void SetBaseColorFactor(const HVector& factor) { m_Data.BaseColorFactor = factor; UpdateData(); };
		void SetMetallic(float metallic) { m_Data.Metallic = metallic; UpdateData(); };
		void SetRoughness(float roughness) { m_Data.Roughness = roughness; UpdateData(); };
		void SetAbientOcclusionFactor(float ao) { m_Data.AbientOcclusionFactor = ao; UpdateData(); };
		void SetEmissiveColor(const Vector& emissiveColor) { m_Data.EmissiveColor = emissiveColor; };
		
		void SetAlbedoTexture(Ref<Texture2D> pTexture) { SetTexture(VT_MATERIAL_SLOTS_BINDING_ALBEDO, pTexture); }
		void SetNormalTexture(Ref<Texture2D> pTexture) { SetTexture(VT_MATERIAL_SLOTS_BINDING_NORMAL, pTexture); }
		void SetRoughnessTexture(Ref<Texture2D> pTexture) { SetTexture(VT_MATERIAL_SLOTS_BINDING_ROUGHNESS, pTexture); }
		void SetMetalllicTexture(Ref<Texture2D> pTexture) { SetTexture(VT_MATERIAL_SLOTS_BINDING_METALLIC, pTexture); }

		void SetTexture(u32 binding, Ref<Texture2D> pTexture);
		void SetName(const std::string& name) { m_Name = name; }
		
		static std::vector<RHI::DescriptorBinding> GetMaterialBindings();



		const VkDescriptorSet& GetSet() const;
	private:
		void UpdateData();

		// Note [07.03.26, Theo]: the uint here stands for the binding in the shader.
		std::string m_Name;

		std::unordered_map<u32, Ref<Texture2D>> m_Textures;

		struct alignas(16) MaterialUBO
		{
			HVector BaseColorFactor{glm::vec4(1.0f)};
			float Metallic{};
			float Roughness{};
			float AbientOcclusionFactor{};
			float _padding0;

			alignas(16) Vector EmissiveColor; 
		};

		Ref<UniformBuffer> m_UBOs[MAX_FRAMES_IN_FLIGHT];
		VkDescriptorSet m_Sets[MAX_FRAMES_IN_FLIGHT];
		MaterialUBO m_Data;
	};

	class MaterialTable
	{
	public:
		MaterialTable(uint32_t materialCount = 1);
		MaterialTable(Ref<MaterialTable> other);
		~MaterialTable() = default;

		bool HasMaterial(uint32_t materialIndex) const { return m_Materials.find(materialIndex) != m_Materials.end(); }
		void SetMaterial(uint32_t index, Ref<Material> material);
		void ClearMaterial(uint32_t index);

		Ref<Material> GetMaterial(uint32_t materialIndex) const
		{
			VT_CORE_ASSERT(HasMaterial(materialIndex), "");
			return m_Materials.at(materialIndex);
		}
		std::map<uint32_t, Ref<Material>>& GetMaterials() { return m_Materials; }
		const std::map<uint32_t, Ref<Material>>& GetMaterials() const { return m_Materials; }

		uint32_t GetMaterialCount() const { return m_MaterialCount; }
		void SetMaterialCount(uint32_t materialCount) { m_MaterialCount = materialCount; }

		void Clear();
	private:
		std::map<uint32_t, Ref<Material>> m_Materials;
		uint32_t m_MaterialCount;
	};
}