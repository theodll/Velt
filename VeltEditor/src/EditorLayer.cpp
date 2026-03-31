#include "EditorLayer.h"
#include <Velt/Velt.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Velt::Editor
{

	EditorLayer::EditorLayer() : Layer("Editor")
	{
		
	}

	void EditorLayer::Init()
	{
		VT_PROFILE_FUNCTION();
		m_ActiveScene = CreateRef<Scene>();

		auto model = m_ActiveScene->CreateEntity("Model");

		model.AddComponent<ModelComponent>("Assets/Models/error.glb");
   	}

	void EditorLayer::OnAttach() 
	{
		VT_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		VT_PROFILE_FUNCTION();

		auto&& camera = SceneRenderer::GetCamera();
        camera->OnUpdate(ts);
        camera->SetViewportSize(ImGuiLayer::GetViewport()->GetWidth(), ImGuiLayer::GetViewport()->GetHeight());

		m_ActiveScene->OnUpdate(ts);
	}

	void EditorLayer::OnEvent(Event& event)
	{
		VT_PROFILE_FUNCTION();
	}

	void EditorLayer::OnRender(VkCommandBuffer commandBuffer)
    {
		VT_PROFILE_FUNCTION();

		m_ActiveScene->OnRender(commandBuffer);
	}

	void EditorLayer::OnImGuiRender2()
	{
		VT_PROFILE_FUNCTION();
	}
}