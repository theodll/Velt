#include "vtpch.h"
#include "Renderer/Renderer.h"
#include "DemoWidget.h"

namespace Velt {
	DemoWidget::DemoWidget(const std::string& name)
		: m_Name(name)
	{
		VT_PROFILE_FUNCTION();
	}

	void DemoWidget::OnRender()
	{
		VT_PROFILE_FUNCTION();
		if (!m_IsVisible)
			return;

		if (ImGui::Begin(m_Name.c_str(), &m_IsVisible, ImGuiWindowFlags_AlwaysAutoResize))
		{
			VT_PROFILE_SCOPE("DemoWidget::OnRender UI");
			ImGui::Text("Hello from Velt ImGui Demo Widget!");
			ImGui::Separator();

			// Float slider
			if (ImGui::SliderFloat("Float Value", &m_FloatValue, 0.0f, 1.0f))
			{
				// Value changed
			}

			// Integer input
			if (ImGui::InputInt("Integer Value", &m_IntValue))
			{
				// Value changed
			}

			// Checkbox
			if (ImGui::Checkbox("Boolean Value", &m_Boovtalue))
			{
				// Value changed
			}

			// Text input
			ImGui::InputText("Text Input", m_TextBuffer, sizeof(m_TextBuffer));


			if (ImGui::Button("Shutdown", ImVec2(120, 0))) 
			{
				RenderAPI::Renderer::requestShutdown();
			}
			// Buttons
			ImGui::Separator();
			if (ImGui::Button("Click Me!", ImVec2(120, 0)))
			{
				// Button clicked
				ImGui::OpenPopup("DemoPopup");
			}

			// Modal popup
			if (ImGui::BeginPopupModal("DemoPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("This is a demo modal popup!");
				ImGui::Spacing();
				if (ImGui::Button("Close", ImVec2(120, 0)))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			// Display current values
			ImGui::Separator();
			ImGui::Text("Current Values:");
			ImGui::BulletText("Float: %.3f", m_FloatValue);
			ImGui::BulletText("Integer: %d", m_IntValue);
			ImGui::BulletText("Boolean: %s", m_Boovtalue ? "true" : "false");
			ImGui::BulletText("Text: %s", m_TextBuffer);

			// Show ImGui demo window toggle
			static bool show_demo_window = false;
			if (ImGui::Checkbox("Show ImGui Demo Window", &show_demo_window))
			{
				// Toggle handled
			}

			if (show_demo_window)
			{
				ImGui::ShowDemoWindow(&show_demo_window);
			}
		}
		ImGui::End();
	}
}
