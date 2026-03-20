#pragma once
#include "Core/Core.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Tools.h"

namespace Velt
{
	// Note [2.02.26, Theo]: Currently every mesh is static (because there straight up is no skeletal
	// mesh/animation system) and I could not bother less to rename this to StaticMesh. 
	// Well get there sometime hopefully (: 

	struct MeshNode
	{
		uint32_t Parent = 0xffffffff;
		std::vector<u32> Children;
		std::vector<u32> Submeshes;

		std::string Name;
		glm::mat4 LocalTransform;

		inline bool IsRoot() const { return Parent == 0xffffffff; }
	};

	// Note [16.03.26, Theo]: this is the "source" file for the mesh. Comparable to MeshSource from Hazel 
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

		std::vector<Ref<Material>>& GetMaterial() { return m_Materials; }
		const std::vector<Ref<Material>>& GetMaterials() const { return m_Materials; }

		const std::filesystem::path& GetFilePath() const { return m_FilePath; }

		Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

	// Node [18.03.26, Theo]: change to private and friend class
	public:
		std::filesystem::path m_FilePath;

		std::vector<Submesh> m_Submeshes;

		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		std::vector<Ref<Material>> m_Materials;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		std::vector<MeshNode> m_Nodes;
	};

	// Note [16.03.26, Theo]: this is the "meta" file for the mesh. Comparable to Mesh from Hazel 
	class VELT_API Model
	{
	public:
		explicit Model(Ref<Mesh> meshSource);
		Model(Ref<Mesh> meshSource, const std::vector<u32>& submeshes);
		virtual ~Model() = default;

		void SetSubmeshes(const std::vector<u32>& submeshes, Ref<Mesh> meshSourceAsset);

		void SetTransform(const TransformComponent& transform) { m_Transform = transform; }
		TransformComponent& GetTransform() { return m_Transform; }
		const TransformComponent& GetTransform() const { return m_Transform; }
		Matrix GetTransformMatrix() const { return m_Transform.Matrix(); }

		void SetTranslation(const Vector& translation) { m_Transform.Translation = translation; }
		void SetScale(const Vector& scale) { m_Transform.Scale = scale; }
		void SetRotationEulerDegrees(const Vector& degrees) { m_Transform.SetEulerDegrees(degrees); }

		Ref<Mesh> GetMeshSource() const { return m_MeshSource; }
		const std::vector<u32>& GetSubmeshes() const { return m_Submeshes; }
		Ref<MaterialTable> GetMaterials() { return m_Materials; }
	public:
		Ref<Mesh> m_MeshSource;
		std::vector<u32> m_Submeshes;

		Ref<MaterialTable> m_Materials;
		TransformComponent m_Transform{};
	};
}