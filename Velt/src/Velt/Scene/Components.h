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
		Ref<Model> model;
		Ref<Mesh> mesh;

		ModelComponent(const std::filesystem::path& path) 
		{
			AssimpMeshImporter importer(path);
			mesh = importer.ImportToMesh();
			model = CreateRef<Velt::Model>(mesh);
		}
		
		ModelComponent(const Ref<Model> pModel)
		{
			model = pModel;
			mesh = pModel->GetMeshSource();
		}

		~ModelComponent() 
		{
			VT_CORE_INFO("Destroy Model Component");
			model.reset();
			mesh.reset();

		}

		ModelComponent() = default;
		ModelComponent(const ModelComponent&) = default;
	};
}