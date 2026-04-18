#pragma once

#include "Scene.h"

namespace Velt 
{
	class VELT_API SceneSerializer 
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