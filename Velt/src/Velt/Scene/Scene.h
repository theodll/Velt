#pragma once
#include "Core/Core.h"
#include <entt/entt.hpp>

#include "Core/Timestep.h"

namespace Velt 
{
	class Entity; 
	namespace Editor {
		class SceneHierarchyPanel;
	}

	class VELT_API Scene 
	{
	public:
		Scene();
		~Scene();

		void Shutdown();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnRender(VkCommandBuffer commandBuffer);

	private:
		entt::registry m_Registry;

	friend class Entity;
	friend class SceneSerializer;
	friend class Velt::Editor::SceneHierarchyPanel;
	};
}