#include "vtpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>

namespace Velt   
{

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("Creating Scene Serializer"); 
	}


	static void SerializeEntity(YAML::Emitter& out, Entity ent) 
	{
		VT_PROFILE_FUNCTION();
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "2093482093840";
		out << YAML::EndMap;

		if (ent.HasComponent<TagComponent>()) 
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			const auto& tag = ent.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			const auto& transform = ent.GetComponent<TransformComponent>();

			out << YAML::Key << "Translation";
			out << YAML::BeginMap;
			out << YAML::Key << "X" << YAML::Value << transform.Translation.x;
			out << YAML::Key << "Y" << YAML::Value << transform.Translation.y;
			out << YAML::Key << "Z" << YAML::Value << transform.Translation.z;
			out << YAML::EndMap;
			
			out << YAML::Key << "Scale";
			out << YAML::BeginMap;
			out << YAML::Key << "X" << YAML::Value << transform.Scale.x;
			out << YAML::Key << "Y" << YAML::Value << transform.Scale.y;
			out << YAML::Key << "Z" << YAML::Value << transform.Scale.z;
			out << YAML::EndMap;

			out << YAML::Key << "Rotation (Euler)";
			out << YAML::BeginMap;
			out << YAML::Key << "X" << YAML::Value << transform.EulerDegrees.x;
			out << YAML::Key << "Y" << YAML::Value << transform.EulerDegrees.y;
			out << YAML::Key << "Z" << YAML::Value << transform.EulerDegrees.z;
			out << YAML::EndMap;

			out << YAML::Key << "Rotation (Quaternion)";
			out << YAML::BeginMap;
			out << YAML::Key << "X" << YAML::Value << transform.Rotation.x;
			out << YAML::Key << "Y" << YAML::Value << transform.Rotation.y;
			out << YAML::Key << "Z" << YAML::Value << transform.Rotation.z;
			out << YAML::Key << "W" << YAML::Value << transform.Rotation.w;
			out << YAML::EndMap;

			out << YAML::EndMap;
		}

		if (ent.HasComponent<ModelComponent>())
		{
			out << YAML::Key << "ModelComponent";
			out << YAML::BeginMap;

			const auto& model = ent.GetComponent<ModelComponent>();

			out << YAML::Key << "Model";
			out << YAML::BeginMap;
			out << YAML::Key << "Mesh Path" << YAML::Value << model.Path.string();;
			out << YAML::EndMap;


			out << YAML::EndMap;
		}

	}

	void SceneSerializer::SerializeText(const std::filesystem::path& path)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("Serializing Text Scene on Path: {}", path.string());
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Name";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (auto entityID : m_Scene->m_Registry.view<entt::entity>())
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;
			SerializeEntity(out, entity);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap; 


		std::filesystem::create_directories(path.parent_path());

		std::ofstream fout(path);
		if (!fout)
			VT_CORE_ERROR("Failed to open or write to {0} when serializing scene", path.string());
		fout << out.c_str();
	}

	void SceneSerializer::SerializeBinary(const std::filesystem::path& path)
	{

	}

	void SceneSerializer::DeserializeText(const std::filesystem::path& path)
	{

	}

	void SceneSerializer::DeserializeBinary(const std::filesystem::path& path)
	{

	}

}