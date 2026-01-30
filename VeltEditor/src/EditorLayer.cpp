#include "EditorLayer.h"
#include "Velt/Core/Input.h"

namespace Editor
{

	EditorLayer::EditorLayer() : Layer("Editor")
	{
		VT_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Velt::Timestep ts)
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("ExampleLayer::Update");

		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_W))
		{
			m_CameraPos.y += 1.f * ts;
		} 
		else if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_A))
		{
			m_CameraPos.x += 1.f * ts;
		} 
		else if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_D))
		{
			m_CameraPos.x -= 1.f * ts;
		}
		else if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_S))
		{
			m_CameraPos.y -= 1.f * ts;
		}

		auto&& camera = Velt::Renderer::SceneRenderer::GetCamera();
		camera->SetPosition(m_CameraPos);


	}

	void EditorLayer::OnEvent(Velt::Event& event)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("{0}", event.ToString());
	}

	void EditorLayer::OnRender(VkCommandBuffer commandBuffer)
	{
		Velt::Renderer::Renderer::DrawQuad(commandBuffer);
	}

	void EditorLayer::OnImGuiRender()
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("ExampleLayer::OnRender");

		// ImGui::Begin("Hello from ExampleLayer");

	}

}