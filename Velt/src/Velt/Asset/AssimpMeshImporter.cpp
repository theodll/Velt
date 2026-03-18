#include "vtpch.h"
#include "AssimpMeshImporter.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Material.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

namespace Velt {

	static const uint32_t s_MeshImportFlags =
		aiProcess_CalcTangentSpace
		| aiProcess_Triangulate
		| aiProcess_SortByPType
		| aiProcess_GenNormals
		| aiProcess_GenUVCoords
		| aiProcess_OptimizeMeshes
		| aiProcess_JoinIdenticalVertices
		| aiProcess_ValidateDataStructure
		| aiProcess_GlobalScale;

	namespace Utils {
		glm::mat4 Mat4FromAIMatrix4x4(const aiMatrix4x4& matrix)
		{
			glm::mat4 result;
			result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
			result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
			result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
			result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
			return result;
		}
	}

	AssimpMeshImporter::AssimpMeshImporter(const std::filesystem::path& path)
		: m_Path(path)
	{
	}

	Ref<Mesh> AssimpMeshImporter::ImportToMesh()
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();

		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene* scene = importer.ReadFile(m_Path.string(), s_MeshImportFlags);
		if (!scene || !scene->HasMeshes())
		{
			return nullptr;
		}

		// Load vertices and indices
		if (scene->HasMeshes())
		{
			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;

			mesh->m_Submeshes.reserve(scene->mNumMeshes);
			for (uint32_t m = 0; m < scene->mNumMeshes; m++)
			{
				aiMesh* aiMesh = scene->mMeshes[m];

				if (!aiMesh->HasPositions() || !aiMesh->HasNormals())
					continue;

				Submesh& submesh = mesh->m_Submeshes.emplace_back();
				submesh.BaseVertex = vertexCount;
				submesh.BaseIndex = indexCount;
				submesh.MaterialIndex = aiMesh->mMaterialIndex;
				submesh.VertexCount = aiMesh->mNumVertices;
				submesh.IndexCount = aiMesh->mNumFaces * 3;
				submesh.MeshName = aiMesh->mName.C_Str();

				vertexCount += aiMesh->mNumVertices;
				indexCount += submesh.IndexCount;

				// Load vertices
				for (uint32_t i = 0; i < aiMesh->mNumVertices; i++)
				{
					Vertex vertex;
					vertex.Position = { aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z };
					vertex.Normal = { aiMesh->mNormals[i].x, aiMesh->mNormals[i].y, aiMesh->mNormals[i].z };

					if (aiMesh->HasTangentsAndBitangents())
					{
						vertex.Tangent = { aiMesh->mTangents[i].x, aiMesh->mTangents[i].y, aiMesh->mTangents[i].z };
						vertex.Binormal = { aiMesh->mBitangents[i].x, aiMesh->mBitangents[i].y, aiMesh->mBitangents[i].z };
					}

					if (aiMesh->HasTextureCoords(0))
						vertex.UV = { aiMesh->mTextureCoords[0][i].x, aiMesh->mTextureCoords[0][i].y };

					mesh->m_Vertices.push_back(vertex);
				}

				// Load indices
				for (uint32_t i = 0; i < aiMesh->mNumFaces; i++)
				{
					Index index = {
						aiMesh->mFaces[i].mIndices[0],
						aiMesh->mFaces[i].mIndices[1],
						aiMesh->mFaces[i].mIndices[2]
					};
					mesh->m_Indices.push_back(index);
				}
			}

			// Traverse scene graph
			MeshNode& rootNode = mesh->m_Nodes.emplace_back();
			TraverseNodes(mesh, scene->mRootNode, 0);
		}

		// Load materials
		if (scene->HasMaterials())
		{
			mesh->m_Materials.resize(scene->mNumMaterials);

			for (uint32_t i = 0; i < scene->mNumMaterials; i++)
			{
				aiMaterial* aiMat = scene->mMaterials[i];

				Ref<Material> material = CreateRef<Material>();
				material->SetName(aiMat->GetName().C_Str());

				// Load basic PBR properties
				aiColor3D color(0.8f, 0.8f, 0.8f);
				aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
				material->SetBaseColorFactor({ color.r, color.g, color.b, 1.0f});

				float roughness = 0.4f;
				aiMat->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness);
				material->SetRoughness(roughness);

				float metalness = 0.0f;
				aiMat->Get(AI_MATKEY_REFLECTIVITY, metalness);
				if (metalness < 0.9f)
					metalness = 0.0f;
				else
					metalness = 1.0f;
				material->SetMetallic(metalness);

				mesh->m_Materials[i] = material;
			}
		}

		// Create buffers
		if (!mesh->m_Vertices.empty())
			mesh->m_VertexBuffer = VertexBuffer::Create(mesh->m_Vertices.data(),
				static_cast<u32>(mesh->m_Vertices.size()), sizeof(Vertex));

		if (!mesh->m_Indices.empty())
			mesh->m_IndexBuffer = IndexBuffer::Create(mesh->m_Indices.data(),
				static_cast<uint32_t>(mesh->m_Indices.size()), sizeof(Index));

		return mesh;
	}

	void AssimpMeshImporter::TraverseNodes(Ref<Mesh> mesh, void* assimpNode, uint32_t nodeIndex,
		const Matrix& parentTransform, uint32_t level)
	{
		aiNode* aNode = static_cast<aiNode*>(assimpNode);

		MeshNode& node = mesh->m_Nodes[nodeIndex];
		node.Name = aNode->mName.C_Str();
		node.LocalTransform = Utils::Mat4FromAIMatrix4x4(aNode->mTransformation);

		Matrix transform = parentTransform * node.LocalTransform;
		for (uint32_t i = 0; i < aNode->mNumMeshes; i++)
		{
			uint32_t submeshIndex = aNode->mMeshes[i];
			auto& submesh = mesh->m_Submeshes[submeshIndex];
			submesh.NodeName = aNode->mName.C_Str();
			submesh.Transform = transform;
			submesh.LocalTransform = node.LocalTransform;

			node.Submeshes.push_back(submeshIndex);
		}

		uint32_t parentNodeIndex = static_cast<uint32_t>(mesh->m_Nodes.size() - 1);
		node.Children.resize(aNode->mNumChildren);
		for (uint32_t i = 0; i < aNode->mNumChildren; i++)
		{
			MeshNode& child = mesh->m_Nodes.emplace_back();
			uint32_t childIndex = static_cast<uint32_t>(mesh->m_Nodes.size() - 1);
			child.Parent = parentNodeIndex;
			mesh->m_Nodes[nodeIndex].Children[i] = childIndex;
			TraverseNodes(mesh, aNode->mChildren[i], childIndex, transform, level + 1);
		}
	}

} // namespace Velt