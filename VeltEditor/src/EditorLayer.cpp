#include "EditorLayer.h"
#include "Velt/Core/Input.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Editor
{

	EditorLayer::EditorLayer() : Layer("Editor")
	{
		
	}

	void EditorLayer::Init()
	{
		VT_CORE_TRACE("goon");

		VT_PROFILE_FUNCTION();

		// Note [5.02.26, Theo] This will be substantially different because all these things we have to do 
		// manually right now will be automatically done by a loader of models eg. glTF or obj.

		std::vector<Velt::Vertex> vertices{
		
			// left face (white)
			{{-.5f, -.5f, -.5f}},
			{{-.5f, .5f, .5f}},
			{{-.5f, -.5f, .5f}},
			{{-.5f, -.5f, -.5f}},
			{{-.5f, .5f, -.5f}},
			{{-.5f, .5f, .5f}},
		
			// right face (yellow)
			{{.5f, -.5f, -.5f}},
			{{.5f, .5f, .5f}},
			{{.5f, -.5f, .5f}},
			{{.5f, -.5f, -.5f}},
			{{.5f, .5f, -.5f}},
			{{.5f, .5f, .5f}},
		
			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}},
			{{.5f, -.5f, .5f}},
			{{-.5f, -.5f, .5f}},
			{{-.5f, -.5f, -.5f}},
			{{.5f, -.5f, -.5f}},
			{{.5f, -.5f, .5f}},
		
			// bottom face (red)
			{{-.5f, .5f, -.5f}},
			{{.5f, .5f, .5f}},
			{{-.5f, .5f, .5f}},
			{{-.5f, .5f, -.5f}},
			{{.5f, .5f, -.5f}},
			{{.5f, .5f, .5f}},
		
			// nose face (blue)
			{{-.5f, -.5f, 0.5f}},
			{{.5f, .5f, 0.5f}},
			{{-.5f, .5f, 0.5f}},
			{{-.5f, -.5f, 0.5f}},
			{{.5f, -.5f, 0.5f}},
			{{.5f, .5f, 0.5f}},
		
			// tail face (green)
			{{-.5f, -.5f, -0.5f}},
			{{.5f, .5f, -0.5f}},
			{{-.5f, .5f, -0.5f}},
			{{-.5f, -.5f, -0.5f}},
			{{.5f, -.5f, -0.5f}},
			{{.5f, .5f, -0.5f}},
		
		};

		std::vector<Velt::Index> indices{
			0,  1,  2,  3,  4,  5,
			6,  7,  8,  9, 10, 11,
			12, 13, 14, 15, 16, 17,
			18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, 29,
			30, 31, 32, 33, 34, 35
		};


		Velt::SubmeshCreateInfo smInfo{};

		smInfo.Vertices = vertices;
		smInfo.Indices = indices;

		Velt::ModelCreateInfo info{};
		info.Parts = { smInfo };
		
		m_Cube = Velt::Model::Create(info);

		m_Cube->GetTransform().translation = { 0.0f, 0.0f, .5f }; 
		m_Cube->GetTransform().scale = { .5f, .5f, .5f };
	}

	void EditorLayer::OnUpdate(Velt::Timestep ts)
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("ExampleLayer::Update");

		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_W))
			m_CameraPos.y -= 1.f * ts;
		
		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_A))
			m_CameraPos.x -= 1.f * ts;
		
		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_D))
			m_CameraPos.x += 1.f * ts;
		
		if (Velt::Input::IsKeyDown(Velt::Scancode::VELT_SCANCODE_S))
			m_CameraPos.y += 1.f * ts;
		
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

		auto&& camera = Velt::SceneRenderer::GetCamera();
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
		// Note [6.02.26]: Never EVER use a VT_PROFILE_FUNCTION() in a per frame performance thing!
		// It causes immense performance loss

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int i{}; i < index; i++)
			Velt::Renderer::DrawStaticModel(commandBuffer, m_Cube);
	}

	void EditorLayer::OnImGuiRender()
	{

		ImGui::Begin("Hello from ExampleLayer");
		if (ImGui::Button("One more cube"))
			index++;

		ImGui::SliderFloat("x", &x, 0.0f, 1.0f);
		ImGui::SliderFloat("y", &y, 0.0f, 1.0f);
		ImGui::SliderFloat("z", &z, 0.0f, 1.0f);
		ImGui::End();

		m_Cube->GetTransform().translation = { x, y, z };
	}

}