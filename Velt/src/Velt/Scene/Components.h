#pragma once
#include "Core/Core.h"
#include "Core/Math.h"
#include "Renderer/Model.h"
#include "Asset/AssimpMeshImporter.h"

#include <string>

namespace Velt
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default; 
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent
	{
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		Vector Translation{ 0.0f };
		Quaternion Rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
		Vector Scale{ 1.0f };
		Vector EulerDegrees{ 0.0f };

		operator Matrix() const { return ToMatrix(); }
		
		void SetEulerDegrees(const Vector& deg)
		{
			EulerDegrees = deg;
			Rotation = glm::quat(glm::radians(deg));
			Rotation = glm::normalize(Rotation);
		}

		Matrix ToMatrix() const
		{
			return glm::translate(glm::mat4(1.0f), Translation)
				* glm::toMat4(Rotation)
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct ModelComponent 
	{
		Ref<Velt::Model> Model;
		Ref<Velt::Mesh> Mesh;

		std::filesystem::path Path;

		ModelComponent(const std::filesystem::path& path) 
		{
			Path = path;
			AssimpMeshImporter importer(path);
			Mesh = importer.ImportToMesh();
			Model = CreateRef<Velt::Model>(Mesh);
		}
		
		ModelComponent(const Ref<Velt::Model> pModel)
		{
			Model = pModel;
			Mesh = pModel->GetMeshSource();
			Path = Mesh->GetFilePath();
		}

		~ModelComponent() 
		{
			VT_CORE_INFO("Destroy Model Component");
			Model.reset();
			Mesh.reset();

		}

		ModelComponent() = default;
		ModelComponent(const ModelComponent&) = default;
	};
}