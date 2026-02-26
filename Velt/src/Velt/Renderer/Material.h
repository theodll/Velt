#pragma once
#include "Core/Core.h"
#include "Core/Math.h"
#include "Platform/Vulkan/DescriptorSetManager.h"

namespace Velt 
{
	struct MaterialUBO
	{
		HVector Color;
	};

	class Material
	{
	public:
		Material(const HVector& color) : m_Color(color) {};

		void SetColor(const HVector& color) { m_Color = color; }

		const Vector& GetColor() const { return m_Color; }
		const VkDescriptorSet& GetSet() const { return m_Set; }
	private:
		// Todo [26.02, Theo]
		HVector m_Color{};
		VkDescriptorSet m_Set;

		std::array<Ref<UniformBuffer>, MAX_FRAMES_IN_FLIGHT> m_UBOs;
		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>    m_Sets;
	};
}