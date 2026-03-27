#pragma once
#include "Core/Core.h"
#include "Core/Math.h"
#include "Platform/Vulkan/DescriptorLayoutCache.h"
#include "Texture.h"
#include <map>
#include <unordered_set>

namespace Velt 
{
	class UniformBuffer;
	class Application;
	

	class VELT_API Material
	{
	public:
		Material();

		void SetBaseColorFactor(const HVector& factor) { m_Data.BaseColorFactor = factor; UpdateData(); };
		void SetMetallic(float metallic) { m_Data.Metallic = metallic; UpdateData(); };
		void SetRoughness(float roughness) { m_Data.Roughness = roughness; UpdateData(); };
		void SetAbientOcclusionFactor(float ao) { m_Data.AbientOcclusionFactor = ao; UpdateData(); };
		void SetEmissiveColor(const Vector& emissiveColor) { m_Data.EmissiveColor = emissiveColor; };
		
		void SetAlbedoTexture(Ref<Texture2D> pTexture) { SetTexture(1, pTexture); VT_CORE_TRACE("Set Albedo Texture"); }
		void SetNormalTexture(Ref<Texture2D> pTexture) { SetTexture(2, pTexture); VT_CORE_TRACE("Set Normal Texture"); }
		void SetRoughnessTexture(Ref<Texture2D> pTexture) { SetTexture(4, pTexture); VT_CORE_TRACE("Set Roughness Texture"); }
		void SetMetalllicTexture(Ref<Texture2D> pTexture) { SetTexture(3, pTexture); VT_CORE_TRACE("Set Roughness Texture");}

		void SetTexture(u32 binding, Ref<Texture2D> pTexture);
		void SetName(const std::string& name) { m_Name = name; }
		
		static std::vector<RHI::DescriptorBinding> GetMaterialBindings();

		const VkDescriptorSet& GetSet() const;
	private:
		void UpdateData();

		// Note [07.03.26, Theo]: the uint here stands for the binding in the shader.
		std::string m_Name;

		std::unordered_map<u32, Ref<Texture2D>> m_Textures;

		// Note [21.03.26, Theo]: kind of hacky because we only use the texture for the sampler and not for the image itself
		Ref<Texture2D> m_Sampler;

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
		std::unordered_set<u32> m_ValidBindings;
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