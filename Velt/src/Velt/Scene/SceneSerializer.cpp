#include "vtpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>

namespace Velt   
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const Vector& v)
	{
		out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const HVector& v)
	{
		out << YAML::Flow << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Quaternion& q)
	{
		out << YAML::Flow << YAML::BeginSeq << q.x << q.y << q.z << q.w << YAML::EndSeq;
		return out;
	}


	static void SerializeEntity(YAML::Emitter& out, Entity ent)
	{
		VT_PROFILE_FUNCTION();
		
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "2093482093840"; 

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

			out << YAML::Key << "Translation" << YAML::Value << transform.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << transform.EulerDegrees;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;
			out << YAML::Key << "Quaternion" << YAML::Value << transform.Rotation;

			out << YAML::EndMap; 
		}

		if (ent.HasComponent<ModelComponent>())
		{
			out << YAML::Key << "ModelComponent";
			out << YAML::BeginMap;

			const auto& model = ent.GetComponent<ModelComponent>();
			out << YAML::Key << "MeshPath" << YAML::Value << model.Path.string();

			out << YAML::EndMap; 
		}

		out << YAML::EndMap;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
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
		VT_PROFILE_FUNCTION();

		std::ifstream fin(path);
		std::stringstream strStream;
		strStream << fin.rdbuf();

		YAML::Node data = YAML::Load(strStream.str()); 
		if (!data["Scene"])
			return;

		std::string sceneName = data["Scene"].as<std::string>();
		VT_CORE_INFO("Deserializing Scene {}", sceneName);

		auto entities = data["Entities"];
		if(entities)
		{
			for (auto entity : entities)
			{
				u64 uuid = entity["Entity"].as<u64>();
				// u64 uuid = 901283018203;
				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				VT_CORE_INFO("Deserializing Entity {0} - {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<Vector>();
					tc.Scale = transformComponent["Scale"].as<Vector>();
					tc.EulerDegrees = transformComponent["Rotation"].as<Vector>();
					tc.Rotation = transformComponent["Quaternion"].as<Quaternion>();
				}

				auto modelComponent = entity["ModelComponent"];
				if (transformComponent)
				{
					ModelComponent mc;
					std::filesystem::path path = modelComponent["MeshPath"].as<std::string>();
					if (deserializedEntity.HasComponent<ModelComponent>())
					{
						deserializedEntity.RemoveComponent<ModelComponent>();
						mc = deserializedEntity.AddComponent<ModelComponent>(path);
					}
					else
						mc = deserializedEntity.AddComponent<ModelComponent>(path);	
				}
			}
		}
	}

	void SceneSerializer::DeserializeBinary(const std::filesystem::path& path)
	{

	}

}