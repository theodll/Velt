#include "vtpch.h"
#include "AssimpMeshImporter.h"

#include "Renderer/VertexBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"
#include "Platform/Vulkan/VulkanContext.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <unordered_map>

namespace Velt {

	static const uint32_t s_MeshImportFlags =
		aiProcess_CalcTangentSpace
		| aiProcess_Triangulate
		| aiProcess_SortByPType
		| aiProcess_GenNormals
		| aiProcess_GenUVCoords
		| aiProcess_JoinIdenticalVertices
		| aiProcess_ValidateDataStructure;

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

		std::filesystem::path ResolveTexturePath(const std::filesystem::path& modelPath, const aiString& texturePath)
		{
			std::filesystem::path path(texturePath.C_Str());
			if (path.is_absolute())
				return path;

			auto resolved = modelPath.parent_path() / path;
			return resolved;
		}

		Ref<Texture2D> LoadMaterialTexture(
			aiMaterial* aiMat,
			const std::filesystem::path& modelPath,
			const std::initializer_list<aiTextureType>& types,
			std::unordered_map<std::string, Ref<Texture2D>>& textureCache)
		{
			VT_PROFILE_FUNCTION();
			for (auto type : types)
			{
				if (aiMat->GetTextureCount(type) == 0)
					continue;

				aiString texturePath;
				if (aiMat->GetTexture(type, 0, &texturePath) != AI_SUCCESS)
					continue;

				auto resolvedPath = ResolveTexturePath(modelPath, texturePath);
				const std::string cacheKey = resolvedPath.lexically_normal().string();

				auto it = textureCache.find(cacheKey);
				if (it != textureCache.end())
					return it->second;

				if (!std::filesystem::exists(resolvedPath))
				{
					VT_CORE_WARN("Material texture not found: {}", resolvedPath.string());
					continue;
				}

				auto texture = Texture2D::Create(resolvedPath);
				if (!texture)
					continue;

				textureCache[cacheKey] = texture;
				return texture;
			}

			return nullptr;
		}
	}

	AssimpMeshImporter::AssimpMeshImporter(const std::filesystem::path& path)
		: m_Path(path)
	{
	}

	Ref<Mesh> AssimpMeshImporter::ImportToMesh()
	{
		VT_PROFILE_FUNCTION();
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
						vertex.UV = { aiMesh->mTextureCoords[0][i].x, 1.0f - aiMesh->mTextureCoords[0][i].y };

					mesh->m_Vertices.push_back(vertex);
				}

				for (uint32_t i = 0; i < aiMesh->mNumFaces; i++)
				{
					const aiFace& face = aiMesh->mFaces[i];

					mesh->m_Indices.push_back(face.mIndices[0]);
					mesh->m_Indices.push_back(face.mIndices[1]);
					mesh->m_Indices.push_back(face.mIndices[2]);
				}
			}

			MeshNode& rootNode = mesh->m_Nodes.emplace_back();
			TraverseNodes(mesh, scene->mRootNode, 0, glm::mat4(1.0f));
		}

		if (scene->HasMaterials())
		{
			mesh->m_Materials.resize(scene->mNumMaterials);
			std::unordered_map<std::string, Ref<Texture2D>> textureCache;

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

				if (auto albedo = Utils::LoadMaterialTexture(aiMat, m_Path, { aiTextureType_BASE_COLOR, aiTextureType_DIFFUSE }, textureCache))
					material->SetAlbedoTexture(albedo);

				if (auto normal = Utils::LoadMaterialTexture(aiMat, m_Path, { aiTextureType_NORMALS, aiTextureType_HEIGHT }, textureCache))
					material->SetNormalTexture(normal);

				if (auto roughnessTexture = Utils::LoadMaterialTexture(aiMat, m_Path, { aiTextureType_DIFFUSE_ROUGHNESS }, textureCache))
					material->SetRoughnessTexture(roughnessTexture);

				if (auto metallicTexture = Utils::LoadMaterialTexture(aiMat, m_Path, { aiTextureType_METALNESS }, textureCache))
					material->SetMetalllicTexture(metallicTexture);

				mesh->m_Materials[i] = material;
			}
		}

		// Create buffers
		if (!mesh->m_Vertices.empty())
			mesh->m_VertexBuffer = VertexBuffer::Create(mesh->m_Vertices.data(),
				static_cast<u32>(mesh->m_Vertices.size()), sizeof(Vertex));

		if (!mesh->m_Indices.empty())
			mesh->m_IndexBuffer = IndexBuffer::Create(mesh->m_Indices.data(),
				static_cast<uint32_t>(mesh->m_Indices.size()));

		if (mesh->m_VertexBuffer || mesh->m_IndexBuffer)
		{
			auto uploader = RHI::VulkanContext::GetResourceUploader();
			uploader->Begin();
			if (mesh->m_VertexBuffer)
				mesh->m_VertexBuffer->Upload(uploader->GetCommandBuffer());
			if (mesh->m_IndexBuffer)
				mesh->m_IndexBuffer->Upload(uploader->GetCommandBuffer());
			uploader->End();
		}

		return mesh;
	}

	void AssimpMeshImporter::TraverseNodes(Ref<Mesh> mesh, aiNode* aNode, uint32_t nodeIndex, const Matrix& parentTransform)
	{
		VT_PROFILE_FUNCTION();
		MeshNode& node = mesh->m_Nodes[nodeIndex];
		node.Name = aNode->mName.C_Str();
		node.LocalTransform = Utils::Mat4FromAIMatrix4x4(aNode->mTransformation);

		Matrix transform = parentTransform * node.LocalTransform;

		for (uint32_t i = 0; i < aNode->mNumMeshes; i++)
		{
			uint32_t submeshIndex = aNode->mMeshes[i];
			auto& submesh = mesh->m_Submeshes[submeshIndex];
			submesh.Transform = transform; 
		}

		node.Children.resize(aNode->mNumChildren);
		for (uint32_t i = 0; i < aNode->mNumChildren; i++)
		{
			uint32_t childIndex = (uint32_t)mesh->m_Nodes.size();
			mesh->m_Nodes.emplace_back();

			mesh->m_Nodes[childIndex].Parent = nodeIndex;
			mesh->m_Nodes[nodeIndex].Children[i] = childIndex;

			TraverseNodes(mesh, aNode->mChildren[i], childIndex, transform);
		}
	}

} // namespace Velt