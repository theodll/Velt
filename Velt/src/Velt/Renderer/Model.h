#pragma once
#include "Mesh.h"

namespace Velt::Renderer
{
	struct Submesh
	{
		Mesh mesh;
		u32 matIndex;
	};

	class VELT_API Model
	{
	public:
		Model() {};
		virtual ~Model() = default;
		const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }
	private: 
		std::vector<Submesh> m_Submeshes;
	};
}