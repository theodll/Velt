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
			out << YAML::Key << "Translation X" << YAML::Value << transform.Translation.x;
			out << YAML::Key << "Translation Y" << YAML::Value << transform.Translation.y;
			out << YAML::Key << "Translation Z" << YAML::Value << transform.Translation.z;

			out << YAML::Key << "Scale" << YAML::Value << transform.Scale.x << YAML::Value << transform.Scale.y << YAML::Value << transform.Scale.z;

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

		std::ofstream fout(path);
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