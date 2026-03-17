#pragma once
#include "Core/Core.h"
#include "Core/Math.h"
#include "Texture.h"
#include <map>

namespace Velt 
{
	class UniformBuffer;
	class Application;

	enum class MaterialTextureSlot : u8
	{
		Albedo = 0,
		Normal = 1,
		Roughness = 2,
		Metallic = 3
	};

	class VELT_API Material
	{
	public:
		Material(const HVector& pColor = HVector(1.0f));

		void SetColor(const HVector& pColor);
		void SetTexture(u32 binding, Ref<Texture2D> pTexture);

		const HVector* GetColor() const { return &m_Data.Color; }
		const VkDescriptorSet& GetSet() const;
	
	private:
		// Todo [26.02.26, Theo]

		// Note [07.03.26, Theo]: the uint here stands for the binding in the shader.

		std::unordered_map<u32, Ref<Texture2D>> m_Textures;

		struct MaterialUBO
		{
			HVector Color{glm::vec4(1.0f)};
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
			VT_CORE_ASSERT(!HasMaterial(materialIndex), "");
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