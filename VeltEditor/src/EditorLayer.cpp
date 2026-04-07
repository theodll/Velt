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

		m_ViewportPanel = CreateRef<Editor::ViewportPanel>();
		m_ViewportPanel->Init(480, 480); 

		m_SceneHierarchyPanel = CreateRef<Editor::SceneHierarchyPanel>();
		m_SceneHierarchyPanel->Init(m_ActiveScene);

		m_EditorGuizmos = CreateRef<EditorGuizmos>();
		m_EditorGuizmos->Init(m_ActiveScene);

		m_EditorCamera = CreateRef<EditorCamera>(glm::radians(50.0f), m_ViewportPanel->GetWidth() / m_ViewportPanel->GetHeight(), 0.1f, 1000.0f);

		m_ViewportPanel->SetEditorCamera(m_EditorCamera);
		m_ViewportPanel->SetEditorGuizmos(m_EditorGuizmos);

		m_SceneRenderer = CreateRef<SceneRenderer>();
		m_SceneRenderer->Init();

		m_DefferedRenderer = CreateRef<DefferedRenderer>();
		m_DefferedRenderer->Init(m_EditorCamera);

		ImGui::SetCurrentContext(GetCurrentImGuiContext());

		auto model = m_ActiveScene->CreateEntity("Model");
		model.AddComponent<ModelComponent>("Assets/Models/error.glb");

		auto model2 = m_ActiveScene->CreateEntity("Model");
		model2.AddComponent<ModelComponent>("Assets/Models/error.glb");
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

		m_Timestep = ts;

        m_EditorCamera->OnUpdate(ts);
		m_EditorCamera->SetViewportSize(m_ViewportPanel->GetWidth(), m_ViewportPanel->GetHeight());
		m_EditorGuizmos->OnUpdate(ts, m_ViewportPanel, m_SceneHierarchyPanel);
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

	static const char* RenderTargetToString(RenderTarget rt)
	{
		switch (rt)
		{
		case VT_RENDER_TARGET_ALBEDO_AO:   return "Albedo + Ambient Occlusion";
		case VT_RENDER_TARGET_NORMAL_ROUGH:   return "Normal + Roughness";
		case VT_RENDER_TARGET_METAL_EMIT:    return "Metal + Emit";
		case VT_RENDER_TARGET_DEPTH:		return "Depth";
		case VT_RENDER_TARGET_COMPOSITE: return "Composite";
		case VT_RENDER_TARGET_MOUSE_PICKING: return "Mouse Picking";
		default: return "Unknown";
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		VT_PROFILE_FUNCTION();
		// Note [04.04.2026, Theo]: dont put anything here because the dockspace might not be set up when calling imgui from here.
	}

	void EditorLayer::OnImGuiRender2()
	{
		VT_PROFILE_FUNCTION();
		m_ViewportPanel->OnImGuiRender2();
		m_EditorGuizmos->OnImGuiRender2();
		m_SceneHierarchyPanel->OnImGuiRender2();

		ImGui::Begin("Statistics");
		ImGui::Text("Velt Engine v0.0");
		ImGui::Separator();
		float v = std::round(1 / m_Timestep);
		static float s = v; s += (v - s) * (1.0f - std::exp(-m_Timestep / 0.12f)); // Smothes the fps display
		ImGui::Text("Frames per Second: %.0fFPS", s);
		ImGui::Dummy({ 500, 3 });
		ImGui::Text("Deltatime (s): %fs", m_Timestep);
		ImGui::Text("Deltatime (ms): %.4gms", m_Timestep.GetMilliseconds());
		ImGui::Dummy({ 500, 3 });
		ImGui::Separator();
		ImGui::Text("Draw Calls: %i", Renderer::GetDrawCallCount());

		static RenderTarget currentRT = (RenderTarget)Application::Get()->SelectedRenderTarget;

		if (ImGui::BeginCombo("Render Target", RenderTargetToString(currentRT)))
		{
			for (int i = 0; i < 6; i++)
			{
				RenderTarget rt = (RenderTarget)i;
				bool selected = (currentRT == rt);

				if (ImGui::Selectable(RenderTargetToString(rt), selected))
				{
					currentRT = rt;
					m_ViewportPanel->PendingRenderTarget = rt;
					m_ViewportPanel->RenderTargetChangePending = true;
				}

				if (selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		//		ImGui::Text("Camera Position: X: %.2f Y: %.2f Z: %.2f", SceneRenderer::GetCamera()->GetPosition().x, SceneRenderer::GetCamera()->GetPosition().y, SceneRenderer::GetCamera()->GetPosition().z);
		ImGui::End(); 
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