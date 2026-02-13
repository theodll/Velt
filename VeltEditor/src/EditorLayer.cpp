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
        index++;

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

    void EditorLayer::OnImGuiRender2()
    {
        ImGui::Begin("Transform");

        if (ImGui::Button("Cube"))
            index++;

        {
            auto& tr = m_Cube->GetTransform();
            m_Translation = tr.translation;
            m_Scale = tr.scale;
        }

        {
            ImGui::PushID("Translation");

            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, 95.0f);
            ImGui::TextUnformatted("Translation");
            ImGui::NextColumn();

            float fullWidth = ImGui::CalcItemWidth();
            float spacing = ImGui::GetStyle().ItemSpacing.x;
            float itemWidth = (fullWidth - 2.0f * spacing) / 3.0f;

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4,4 });

            float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

            ImGui::PushStyleColor(ImGuiCol_Button, { 0.80f,0.10f,0.15f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.90f,0.20f,0.25f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.00f,0.30f,0.35f,1 });
            if (ImGui::Button("X", buttonSize)) m_Translation.x = 0.0f;
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::PushItemWidth(itemWidth);
            ImGui::DragFloat("##TX", &m_Translation.x, 0.05f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, { 0.20f,0.70f,0.20f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.30f,0.80f,0.30f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.40f,0.90f,0.40f,1 });
            if (ImGui::Button("Y", buttonSize)) m_Translation.y = 0.0f;
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::PushItemWidth(itemWidth);
            ImGui::DragFloat("##TY", &m_Translation.y, 0.05f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, { 0.10f,0.25f,0.80f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.20f,0.35f,0.90f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.30f,0.45f,1.00f,1 });
            if (ImGui::Button("Z", buttonSize)) m_Translation.z = 0.0f;
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::PushItemWidth(itemWidth);
            ImGui::DragFloat("##TZ", &m_Translation.z, 0.05f);
            ImGui::PopItemWidth();

            ImGui::PopStyleVar();
            ImGui::Columns(1);
            ImGui::PopID();
        }

        {
            ImGui::PushID("Rotation");

            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, 95.0f);
            ImGui::TextUnformatted("Rotation");
            ImGui::NextColumn();

            float fullWidth = ImGui::CalcItemWidth();
            float spacing = ImGui::GetStyle().ItemSpacing.x;
            float itemWidth = (fullWidth - 2.0f * spacing) / 3.0f;

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4,4 });

            float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

            ImGui::PushStyleColor(ImGuiCol_Button, { 0.80f,0.10f,0.15f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.90f,0.20f,0.25f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.00f,0.30f,0.35f,1 });
            if (ImGui::Button("X", buttonSize)) m_Rotation.x = 0.0f;
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::PushItemWidth(itemWidth);
            ImGui::DragFloat("##RX", &m_Rotation.x, 0.5f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, { 0.20f,0.70f,0.20f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.30f,0.80f,0.30f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.40f,0.90f,0.40f,1 });
            if (ImGui::Button("Y", buttonSize)) m_Rotation.y = 0.0f;
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::PushItemWidth(itemWidth);
            ImGui::DragFloat("##RY", &m_Rotation.y, 0.5f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, { 0.10f,0.25f,0.80f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.20f,0.35f,0.90f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.30f,0.45f,1.00f,1 });
            if (ImGui::Button("Z", buttonSize)) m_Rotation.z = 0.0f;
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::PushItemWidth(itemWidth);
            ImGui::DragFloat("##RZ", &m_Rotation.z, 0.5f);
            ImGui::PopItemWidth();

            ImGui::PopStyleVar();
            ImGui::Columns(1);
            ImGui::PopID();
        }

        {
            ImGui::PushID("Scale");

            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, 95.0f);
            ImGui::TextUnformatted("Scale");
            ImGui::NextColumn();

            float fullWidth = ImGui::CalcItemWidth();
            float spacing = ImGui::GetStyle().ItemSpacing.x;
            float itemWidth = (fullWidth - 2.0f * spacing) / 3.0f;

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 4,4 });

            float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
            ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

            ImGui::PushStyleColor(ImGuiCol_Button, { 0.80f,0.10f,0.15f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.90f,0.20f,0.25f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.00f,0.30f,0.35f,1 });
            if (ImGui::Button("X", buttonSize)) m_Scale.x = 1.0f;
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::PushItemWidth(itemWidth);
            ImGui::DragFloat("##SX", &m_Scale.x, 0.05f, 0.001f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, { 0.20f,0.70f,0.20f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.30f,0.80f,0.30f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.40f,0.90f,0.40f,1 });
            if (ImGui::Button("Y", buttonSize)) m_Scale.y = 1.0f;
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::PushItemWidth(itemWidth);
            ImGui::DragFloat("##SY", &m_Scale.y, 0.05f, 0.001f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, { 0.10f,0.25f,0.80f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.20f,0.35f,0.90f,1 });
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.30f,0.45f,1.00f,1 });
            if (ImGui::Button("Z", buttonSize)) m_Scale.z = 1.0f;
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ImGui::PushItemWidth(itemWidth);
            ImGui::DragFloat("##SZ", &m_Scale.z, 0.05f, 0.001f);
            ImGui::PopItemWidth();

            ImGui::PopStyleVar();
            ImGui::Columns(1);
            ImGui::PopID();
        }

        ImGui::End();

        auto& tr = m_Cube->GetTransform();

        m_Scale.x = (m_Scale.x < 0.001f) ? 0.001f : m_Scale.x;
        m_Scale.y = (m_Scale.y < 0.001f) ? 0.001f : m_Scale.y;
        m_Scale.z = (m_Scale.z < 0.001f) ? 0.001f : m_Scale.z;

        tr.translation = m_Translation;
        tr.scale = m_Scale;
        tr.SetEulerDegrees(m_Rotation);
    }


}