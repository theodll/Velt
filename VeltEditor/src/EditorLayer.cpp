#include "EditorLayer.h"
#include "Velt/Core/Input.h"
#include <glm/gtc/matrix_transform.hpp>

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
			m_CameraPos.y += 1.f * ts;
		
		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_A))
			m_CameraPos.x += 1.f * ts;
		
		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_D))
			m_CameraPos.x -= 1.f * ts;
		
		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_S))
			m_CameraPos.y -= 1.f * ts;
		
		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_Q))
			m_CameraRot += 100.f * ts;

		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_E))
			m_CameraRot-= 100.f * ts;

		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_I))
			m_SquarePos.y -= 1.f * ts;

		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_J))
			m_SquarePos.x -= 1.f * ts;

		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_L))
			m_SquarePos.x += 1.f * ts;

		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_K))
			m_SquarePos.y += 1.f * ts;

		auto&& camera = Velt::Renderer::SceneRenderer::GetCamera();
		camera->SetPosition(m_CameraPos);
		camera->SetRotation(m_CameraRot);
	}

	void EditorLayer::OnEvent(Velt::Event& event)
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("{0}", event.ToString());
	}

	void EditorLayer::OnRender(VkCommandBuffer commandBuffer)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePos);

		Velt::Renderer::Renderer::DrawQuad(commandBuffer);
		Velt::Renderer::Renderer::DrawQuad(commandBuffer, transform);
	}

	void EditorLayer::OnImGuiRender()
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("ExampleLayer::OnRender");

		// ImGui::Begin("Hello from ExampleLayer");

	}

}