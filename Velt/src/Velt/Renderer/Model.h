#pragma once
#include "Mesh.h"
#include "Tools.h"

namespace Velt
{
	// Note [16.03.26, Theo]: this is the "meta" file for the mesh. Comperable to Mesh from Hazel 
	class VELT_API Model
	{
	public:
		explicit Model(Ref<Mesh> meshSource);
		Model(Ref<Mesh> meshSource, const std::vector<uint32_t>& submeshes);
		virtual ~Model() = default;

		TransformComponent& GetTransform() { return m_Transform; }
		const std::vector<u32>& GetSubmeshes() const { return m_Submeshes; }
	private:
		std::vector<u32> m_Submeshes;
		TransformComponent m_Transform{};


	};
}