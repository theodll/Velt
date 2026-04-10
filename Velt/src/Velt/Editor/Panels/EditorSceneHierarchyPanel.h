#pragma once
#include <queue>
#include "Core/Core.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "../EditorPanel.h"

namespace Velt::Editor 
{
	class VELT_API SceneHierarchyPanel : public EditorPanel 
	{

	public:
		SceneHierarchyPanel() = default;
		~SceneHierarchyPanel() = default;

		void Init(const Ref<Scene>& sceneContext);
		void Shutdown() override;

		void SetContext(const Ref<Scene>& sceneContext);
		void SetSelected(Entity selected);

		Entity GetSelected() const { return m_SelectionContext; }

		void OnUpdate(Timestep ts) override;
		void OnImGuiRender2() override;
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents();

		Ref<Scene> m_ContextScene;
		std::queue<Entity> m_QueueDeleteEntities;
		std::queue<std::filesystem::path> m_QueueRecreateModelComponents; 
		Entity m_SelectionContext;
	};
}