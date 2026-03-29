#include "EditorLayer.h"
#include "Velt/Core/Input.h"
#include "Velt/Core/Application.h"
#include "Velt/Renderer/Material.h"
#include "Velt/Asset/AssimpMeshImporter.h"
#include "Core/Math.h"
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Editor
{

	EditorLayer::EditorLayer() : Layer("Editor")
	{
		
	}

	void EditorLayer::Init()
	{
		VT_PROFILE_FUNCTION();
        

        auto camera = Velt::SceneRenderer::GetCamera(); 

        //Velt::AssimpMeshImporter importer("Assets/Models/Sphere/PlasticSphere.gltf");
        Velt::AssimpMeshImporter importer("Assets/Models/Sponza/Sponza.gltf");
        //Velt::AssimpMeshImporter importer("Assets/Models/Helmet/SciFiHelmet.gltf");
        m_Mesh = importer.ImportToMesh();

        m_Model = CreateRef<Velt::Model>(m_Mesh);
        //m_Model->SetRotationEulerDegrees({ 180.0f, 180.0f, 0.0f });
        m_Model->SetScale(Velt::Vector(1.0f));
		m_Model->SetTranslation({ 0.0f, -1.0f, 0.0f });

     //   camera->SetViewDirection(Velt::Vector(0.0f), Velt::Vector(0.5f, 0.0f, 1.0f));
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
        camera->OnUpdate(ts);
        camera->SetViewportSize(Velt::ImGuiLayer::GetViewport()->GetWidth(), Velt::ImGuiLayer::GetViewport()->GetHeight());
//		camera->SetPosition(m_CameraPos);
	//	camera->SetRotation(m_CameraRot);

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

		if (!m_Model || !m_Mesh)
			return;

		const auto& submeshes = m_Model->GetSubmeshes();
		auto materialTable = m_Model->GetMaterials();
		for (u32 submeshIndex : submeshes)
		{
			Velt::Renderer::DrawStaticModel(
				commandBuffer,
				Velt::SceneRenderer::GetPipeline(),
				m_Model,
				m_Mesh,
				submeshIndex,
				materialTable
			);
		}

	   // Velt::Renderer::DrawQuad(commandBuffer, glm::mat4(1.0f), *m_Material.get());
	}

    void EditorLayer::OnImGuiRender2()
    {
        /*
        ImGui::Begin("Transform");

        if (ImGui::Button("Cube"))
            index++;

        {
            m_Translation = tr.Translation;
            m_Scale = tr.Scale;
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



        tr.Translation = m_Translation;
        tr.Scale = m_Scale;
        tr.SetEulerDegrees(m_Rotation);

        ImGui::Begin("Material Settings");
        {
            ImGui::PushID("Material");

            const Velt::HVector* color = m_Material->GetColor();
            float colorArray[4] = { color->x, color->y, color->z, color->w };

            ImGui::Columns(2);
            ImGui::SetColumnWidth(0, 95.0f);
            ImGui::TextUnformatted("Color (RGBA)");
            ImGui::NextColumn();

            if (ImGui::ColorEdit4("##RGBA", colorArray))
            {
                m_Material->SetColor(Velt::HVector(colorArray[0], colorArray[1], colorArray[2], colorArray[3]));
            }

            ImGui::Columns(1);
            ImGui::PopID();
        }
        ImGui::End();
    }


    */
    }
}