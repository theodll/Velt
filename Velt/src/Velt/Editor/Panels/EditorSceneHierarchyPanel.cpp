#include "vtpch.h"
#include "EditorSceneHierarchyPanel.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Velt::Editor
{
	void SceneHierarchyPanel::Init(const Ref<Scene>& sceneContext)
	{
		SetContext(sceneContext);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& sceneContext)
	{
		m_ContextScene = sceneContext;
	}

	void SceneHierarchyPanel::SetSelected(Entity selected)
	{
		m_SelectionContext = selected;
	}


	void SceneHierarchyPanel::Shutdown()
	{
	}

	void SceneHierarchyPanel::OnUpdate(Timestep ts)
	{
		
	}

	void SceneHierarchyPanel::OnImGuiRender2()
	{
		ImGui::Begin("Scene Hierarchy Panel");


		for (auto entityID : m_ContextScene->m_Registry.view<entt::entity>())
		{
			Entity entity{ entityID, m_ContextScene.get() };
			DrawEntityNode(entity);
		}
			
		ImGui::End();

		ImGui::Begin("Auditor");
		DrawComponents();
		ImGui::End();

	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = (m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0| ImGuiTreeNodeFlags_OpenOnArrow;

		bool open = ImGui::TreeNodeEx((void*)(u64)(u32)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (open)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponents()
	{
		if (!m_SelectionContext)
			return;
		
		if (m_SelectionContext.HasComponent<TagComponent>())
		{
			auto& tag = m_SelectionContext.GetComponent<TagComponent>().Tag;

			char buffer[512];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer))) 
			{
				tag = std::string(buffer);
			}
		}

		if (m_SelectionContext.HasComponent<TransformComponent>())
		{
			auto& transform = m_SelectionContext.GetComponent<TransformComponent>();
			if (ImGui::DragFloat3("Position", glm::value_ptr(transform.Translation), 0.5f)) 
			{

			}
		}

		if (m_SelectionContext.HasComponent<ModelComponent>())
		{
		}

	}

}