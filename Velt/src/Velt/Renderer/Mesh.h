#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Velt::Renderer
{
	// Note [2.02.26, Theo]: Currently every mesh is static (because there straight up is no skeletal
	// mesh/animation system) and I could not bother less to rename this to StaticMesh. 
	// Well get there sometime hopefully (: 

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices);
		virtual ~Mesh() = default;

		Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; };
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; };
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;
		
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}
