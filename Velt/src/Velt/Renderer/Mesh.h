#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Velt
{
	// Note [2.02.26, Theo]: Currently every mesh is static (because there straight up is no skeletal
	// mesh/animation system) and I could not bother less to rename this to StaticMesh. 
	// Well get there sometime hopefully (: 

	// Note [16.03.26, Theo]: this is the "source" file for the mesh. Comperable to MeshSource from Hazel 
	struct Submesh
	{
		u32 BaseVertex; 
		u32 BaseIndex;
		u32 MaterialIndex;
		u32 IndexCount;
		u32 VertexCount;

		Matrix Transform{ 1.0f };
		Matrix LocalTransform{ 1.0f };
	};

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const Matrix& transform);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<Submesh>& submeshes);
		virtual ~Mesh() = default;

		std::vector<Submesh>& GetSubmeshes() { return m_Submeshes; }
		const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }

		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		const std::vector<Index>& GetIndices() const { return m_Indices; }

		std::vector<Material>& GetMaterial() { return m_Materials; }
		const std::vector<Material>& GetMaterials() const { return m_Materials; }

		const std::filesystem::path& GetFilePath() const { return m_FilePath; }

		Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

	private:
		std::filesystem::path m_FilePath;

		std::vector<Submesh> m_Submeshes;

		std::vector<Vertex> m_Vertices; 
		std::vector<Index> m_Indices;

		std::vector<Material> m_Materials;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

	};

}
