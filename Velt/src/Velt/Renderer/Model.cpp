#include "Core/Core.h"
#include "Model.h"

namespace Velt {


	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform) : m_Vertices(vertices), m_Indices(indices)
	{
		Submesh submesh;
		submesh.BaseVertex = 0;
		submesh.BaseIndex = 0;
		submesh.IndexCount = (uint32_t)indices.size() * 3u;
		submesh.Transform = transform;
		m_Submeshes.push_back(submesh);

		m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), (uint32_t)(m_Vertices.size()), sizeof(Vertex));
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32_t)(m_Indices.size() * sizeof(Index)));
	}

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<Submesh>& submeshes)
		: m_Vertices(vertices), m_Indices(indices), m_Submeshes(submeshes)
	{
		m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), (uint32_t)(m_Vertices.size()), sizeof(Vertex));
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32_t)(m_Indices.size() * sizeof(Index)));
	}

	Model::Model(Ref<Mesh> meshSource) : m_MeshSource(meshSource)
	{
		m_Materials = CreateRef<MaterialTable>(0);

		if (meshSource)
		{
			SetSubmeshes({}, meshSource);

			const std::vector<Ref<Material>>& meshMaterials = meshSource->GetMaterials();
			for (size_t i = 0; i < meshMaterials.size(); i++)
				m_Materials->SetMaterial((uint32_t)i, meshMaterials[i]);
		}
	}

	Model::Model(Ref<Mesh> meshSource, const std::vector<uint32_t>& submeshes) : m_MeshSource(meshSource)
	{
		m_Materials = CreateRef<MaterialTable>(0);

		if (meshSource)
		{
			SetSubmeshes(submeshes, meshSource);

			const std::vector<Ref<Material>>& meshMaterials = meshSource->GetMaterials();
			for (size_t i = 0; i < meshMaterials.size(); i++)
				m_Materials->SetMaterial((uint32_t)i, meshMaterials[i]);
		}
	}

	void Model::SetSubmeshes(const std::vector<u32>& submeshes, Ref<Mesh> meshSource)
	{
		if (!submeshes.empty())
		{
			m_Submeshes = submeshes;
		}
		else
		{
			const auto& submeshes = meshSource->GetSubmeshes();
			m_Submeshes.resize(submeshes.size());
			for (u32 i = 0; i < submeshes.size(); i++)
				m_Submeshes[i] = i;
		}
	}

}