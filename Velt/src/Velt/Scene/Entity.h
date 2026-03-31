#pragma once
#include "Core/Core.h"
#include "Scene.h"
#include <entt/entt.hpp>

namespace Velt 
{
	class VELT_API Entity 
	{
	public: 
		Entity(const Entity&) = default;

		Entity(entt::entity handle, Scene* pScene) : m_EntityHandle(handle), m_pContext(pScene)
		{
			VT_PROFILE_FUNCTION();
		}

		template<typename T>
		bool HasComponent()
		{
			VT_PROFILE_FUNCTION();
			return m_pContext->m_Registry.any_of<T>(m_EntityHandle);
		}


		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			VT_PROFILE_FUNCTION();
			VT_CORE_ASSERT(!HasComponent<T>(), "Entity already has this component");
			return m_pContext->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}


		template<typename T>
		T& GetComponent()
		{
			VT_PROFILE_FUNCTION();
			VT_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component");
			return m_pContext->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			VT_PROFILE_FUNCTION();
			VT_CORE_ASSERT(HasComponent<T>(), "Entity does not have this component");
			m_pContext->m_Registry.remove<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_pContext = nullptr;
	};


}