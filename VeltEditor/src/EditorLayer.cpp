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

		m_SceneRenderer = CreateRef<SceneRenderer>();
		m_SceneRenderer->Init();

		m_ViewportPanel = CreateRef<EditorViewportPanel>();
		m_ViewportPanel->Init(480, 480); 

		float width, height{};

		if (m_ViewportPanel)
		{
			width = m_ViewportPanel->GetWidth();
			height = m_ViewportPanel->GetHeight();
		}
		else
		{
			width = 1920;
			height = 1080;
		}

		float aspect = width / height;

		m_EditorCamera = CreateRef<EditorCamera>(glm::radians(50.0f), aspect, 0.1f, 1000.0f); 

		m_DefferedRenderer = CreateRef<DefferedRenderer>();
		m_DefferedRenderer->Init(m_EditorCamera);

		auto model = m_ActiveScene->CreateEntity("Model");
		model.AddComponent<ModelComponent>("Assets/Models/error.glb");
   	}

	void EditorLayer::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("Shutdown EditorLayer");

		m_SceneRenderer->Shutdown();
		m_SceneRenderer.reset();

		m_DefferedRenderer->Shutdown();
		m_DefferedRenderer.reset();

		m_ActiveScene->Shutdown();
		m_ActiveScene.reset();
		
		m_ViewportPanel->Shutdown();
		m_ViewportPanel.reset();
	}

	void EditorLayer::OnAttach() 
	{
		VT_PROFILE_FUNCTION();
	}

	void EditorLayer::OnDetach()
	{
		Shutdown();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		VT_PROFILE_FUNCTION();

        m_EditorCamera->OnUpdate(ts);
		m_EditorCamera->SetViewportSize(m_ViewportPanel->GetWidth(), m_ViewportPanel->GetHeight());

		m_ActiveScene->OnUpdate(ts);
	}

	void EditorLayer::OnEvent(Event& event)
	{
		VT_PROFILE_FUNCTION();
	}

	void EditorLayer::OnRender(VkCommandBuffer commandBuffer)
    {
		VT_PROFILE_FUNCTION();
		
		m_SceneRenderer->BeginScene(m_EditorCamera);

		m_ActiveScene->OnRender(commandBuffer);

		m_SceneRenderer->EndScene();
	}

	void EditorLayer::OnImGuiRender2()
	{
		VT_PROFILE_FUNCTION();
		m_ViewportPanel->OnImGuiRender();
	}

	void EditorLayer::OnDefferedRender(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();
		m_DefferedRenderer->ExecuteDefferedPass(commandBuffer);
	}

	void EditorLayer::OnBeforeFrameBegin()
	{
		VT_PROFILE_FUNCTION();
		m_ViewportPanel->ProcessPendingChanges();
	}

}