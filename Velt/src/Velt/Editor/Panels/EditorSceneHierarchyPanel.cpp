#include "vtpch.h"
#include "EditorSceneHierarchyPanel.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Editor/FontLibrary/FontLibrary.h"

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
		VT_PROFILE_FUNCTION();
		while (!m_QueueDeleteEntities.empty())
		{
			VT_CORE_INFO("Delete Queued Entity with ID: {0}", m_QueueDeleteEntities.front().id());
			m_ContextScene->DestroyEntity(m_QueueDeleteEntities.front());
			m_QueueDeleteEntities.pop();
		}
	}

	void SceneHierarchyPanel::OnImGuiRender2()
	{
		ImGui::Begin("Scene Hierarchy Panel");


		if (ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_ContextScene->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		for (auto entityID : m_ContextScene->m_Registry.view<entt::entity>())
		{
			Entity entity{ entityID, m_ContextScene.get() };
			DrawEntityNode(entity);
		}
			
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		ImGui::End();

		ImGui::Begin("Auditor");
		DrawComponents();
		ImGui::End();

	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth; 

		bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_QueueDeleteEntities.push(std::move(entity));
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
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

			if (ImGui::BeginTable("TagTable", 2))
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("ID: %i", (u32)m_SelectionContext);

				ImGui::TableNextColumn();
				ImGui::PushItemWidth(-1);
				//ImGui::PushStyleVar(ImGuiStyleVar_)
				if (ImGui::InputText("", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
				ImGui::PopItemWidth();

				ImGui::EndTable();
			}
		}

		if (m_SelectionContext.HasComponent<TransformComponent>())
		{
			FontLibrary::Get().Push(VT_FONT_TYPE_SPECIAL_BOLD);
			bool open = ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed);
			FontLibrary::Get().Pop();

			if (open)
			{
				auto& tc = m_SelectionContext.GetComponent<TransformComponent>();
				
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 1.0f, 1.0f });

				Shared::DrawVec3Control("Translation", tc.Translation);
				Shared::DrawVec3Control("Rotation", tc.EulerDegrees);
				Shared::DrawVec3Control("Scale", tc.Scale);

				ImGui::PopStyleVar();

				ImGui::TreePop(); 
			}
		}

		if (m_SelectionContext.HasComponent<ModelComponent>())
		{
			FontLibrary::Get().Push(VT_FONT_TYPE_ICON);
			//ImGui::Text(VT_ICON_FA_);
			FontLibrary::Get().Pop(); 


			FontLibrary::Get().Push(VT_FONT_TYPE_SPECIAL_BOLD);
			bool open = ImGui::TreeNodeEx("Model", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed);
			FontLibrary::Get().Pop();

			if (open)
			{
				auto& model = m_SelectionContext.GetComponent<ModelComponent>();

				if (ImGui::BeginTable("TransformTable", 2))
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Path: %s", model.Mesh->GetFilePath().c_str());

					ImGui::TableNextColumn();
					ImGui::PushItemWidth(-1);
					
					ImGui::PopItemWidth();

					ImGui::EndTable();
				}

				ImGui::TreePop();
			}
		}

	}

}