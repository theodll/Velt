#include "Mesh.h"

namespace Velt::Renderer
{
	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices) 
		: m_Vertices(vertices), m_Indices(indices)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Create Mesh");

		m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), m_Vertices.size(), m_Vertices.size() * sizeof(Vertex), true);
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size(), 0, true);
	}
}