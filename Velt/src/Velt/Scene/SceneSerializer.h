#pragma once

#include "Core/Core.h"
#include "Scene.h"
#include "Core/Math.h"
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<Velt::Vector>  
	{
		static Node encode(const Velt::Vector& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, Velt::Vector& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Velt::HVector>  
	{
		static Node encode(const Velt::HVector& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, Velt::HVector& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Velt::Quaternion>  
	{
		static Node encode(const Velt::Quaternion& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}
		static bool decode(const Node& node, Velt::Quaternion& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace Velt 
{
	class __declspec(dllexport) SceneSerializer 
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void SerializeText(const std::filesystem::path& path);
		void SerializeBinary(const std::filesystem::path& path);


		void DeserializeText(const std::filesystem::path& path);
		void DeserializeBinary(const std::filesystem::path& path);
	private:
		Ref<Scene> m_Scene;
	};
}