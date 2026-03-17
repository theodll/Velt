#pragma once
#include "Core/Core.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

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

		std::string NodeName, MeshName;
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
		const std::vector<Ref<Material>>& GetMaterials() const { return m_Materials; }

		const std::filesystem::path& GetFilePath() const { return m_FilePath; }

		Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

	private:
		std::filesystem::path m_FilePath;

		std::vector<Submesh> m_Submeshes;

		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		std::vector<Ref<Material>> m_Materials;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

	};

	// Note [16.03.26, Theo]: this is the "meta" file for the mesh. Comperable to Mesh from Hazel 
	class VELT_API Model
	{
	public:
		explicit Model(Ref<Mesh> meshSource);
		Model(Ref<Mesh> meshSource, const std::vector<uint32_t>& submeshes);
		virtual ~Model() = default;

		void SetSubmeshes(const std::vector<u32>& submeshes, Ref<Mesh> meshSourceAsset);

		Ref<Mesh> GetMeshSource() const { return m_MeshSource; }
		const std::vector<u32>& GetSubmeshes() const { return m_Submeshes; }
		Ref<MaterialTable> GetMaterials() { return m_Materials; }
	private:
		Ref<Mesh> m_MeshSource;
		std::vector<u32> m_Submeshes;

		Ref<MaterialTable> m_Materials;
	};
}