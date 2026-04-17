#include "vtpch.h"
#include "SceneSerializer.h"
#include "Entity.h"
#include <yaml-cpp/yaml.h>

namespace Velt   
{

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
	{

	}

	void SceneSerializer::SerializeText(const std::filesystem::path& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Name";
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;

		for (auto entityID : m_Scene->m_Registry.view<entt::entity>())
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap; 

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