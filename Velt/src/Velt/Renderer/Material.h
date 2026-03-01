#pragma once
#include "Core/Core.h"
#include "Core/Math.h"

namespace Velt 
{
	class UniformBuffer;
	class Application;

	struct MaterialUBO
	{
		HVector Color;
	};

	class VELT_API Material
	{
	public:
		Material(const HVector* pColor);

		void SetColor(const HVector* pColor);

		const HVector* GetColor() const { return &m_Data.Color; }
		const VkDescriptorSet& GetSet() const;
	
	private:
		// Todo [26.02, Theo]

		Ref<UniformBuffer> m_UBOs[MAX_FRAMES_IN_FLIGHT];
		VkDescriptorSet m_Sets[MAX_FRAMES_IN_FLIGHT];
		MaterialUBO m_Data;
	};
}