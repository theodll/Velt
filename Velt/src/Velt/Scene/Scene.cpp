#include "vtpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer.h"
#include "Components.h"

namespace Velt 
{

	Scene::Scene()
	{
		VT_PROFILE_FUNCTION();
	}

	Scene::~Scene()
	{
		VT_PROFILE_FUNCTION();
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		VT_PROFILE_FUNCTION();
		Entity entity = { m_Registry.create(), this }; 
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name; 
		return  entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		VT_PROFILE_FUNCTION();
	}

	void Scene::OnRender(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();

		auto group = m_Registry.group<TransformComponent>(entt::get<ModelComponent>);
		for (auto entity : group) 
		{
			auto&& [transform, model] = group.get<TransformComponent, ModelComponent>(entity);
			
			const auto& submeshes = model.model->GetSubmeshes();
			auto materialTable = model.model->GetMaterials();
			for (u32 submeshIndex : submeshes)
			{
				Renderer::DrawStaticModel(
					commandBuffer,
					SceneRenderer::GetPipeline(),
					model.model,
					model.mesh,
					submeshIndex,
					materialTable,
					transform
				);
			}
		
		}
	}
}