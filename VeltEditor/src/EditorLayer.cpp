#include "EditorLayer.h"
#include "Velt/Core/Input.h"
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

		Velt::Renderer::Vertex quadVerticesData[] = {
			{ {-0.5f, -0.5f} },
			{ { 0.5f, -0.5f} },
			{ { 0.5f,  0.5f} },
			{ {-0.5f,  0.5f} }
		};

		std::vector<Velt::Renderer::Vertex> quadVertices(quadVerticesData, quadVerticesData + 4);
		std::vector<Velt::Renderer::Index> quadIndices = { 0, 1, 2, 2, 3, 0 };

		Velt::Renderer::SubmeshCreateInfo smInfo{};

		smInfo.Vertices = quadVertices;
		smInfo.Indices = quadIndices;

		Velt::Renderer::ModelCreateInfo info{};
		info.Parts = { smInfo };
		
		m_Model = Velt::Renderer::Model::Create(info);
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
		// Note [6.02.26]: Never EVER use a VT_PROFILE_FUNCTION() in a per frame performance thing!
		// It causes immense performance loss

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int x{}; x < 100; x++)
		{
			for (int y{}; y < 100; y++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Velt::Renderer::Renderer::DrawStaticModel(commandBuffer, m_Model, transform);
			}
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("ExampleLayer::OnRender");
		// ImGui::Begin("Hello from ExampleLayer");
	}

}