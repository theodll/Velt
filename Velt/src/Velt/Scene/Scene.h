#pragma once
#include "Core/Core.h"
#include <entt/entt.hpp>

#include "Core/Timestep.h"

namespace Velt 
{
	class Entity; 
	class VELT_API Scene 
	{
	public:
		Scene();
		~Scene();

		void Shutdown();

		Entity CreateEntity(const std::string& name = std::string());

		void OnUpdate(Timestep ts);
		void OnRender(VkCommandBuffer commandBuffer);

		entt::registry& Reg() { return m_Registry; };
	private:
		entt::registry m_Registry;

	friend class Entity;
	};
}