#pragma once
#include "Core/Core.h"
#include "Core/Timestep.h"
#include "FontLibrary/FontLibrary.h"

#include "Core/Math.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Velt::Editor 
{
	class EditorPanel 
	{
	public: 
		EditorPanel() = default;
		virtual ~EditorPanel() = default;

		virtual void Init() {};
		virtual void Shutdown() {}; 

		virtual void OnUpdate(Timestep ts) {};

		virtual void OnImGuiRender2() {};
	};

	namespace Shared 
	{
		static void DrawVec3Control(const std::string& label, Vector& values, float resetValue = 0.0f, float columnWidth = 100.0f, float speed = 0.05f)
		{
			ImGui::PushID(label.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::TextUnformatted(label.c_str());
			ImGui::NextColumn();

			float fullWidth = ImGui::GetContentRegionAvail().x;
			float spacing = 4.0f; 
			float itemWidth = (fullWidth - 2.0f * spacing) / 3.0f;

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ spacing, 4.0f });

			float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.80f, 0.10f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.90f, 0.20f, 0.25f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1.00f, 0.30f, 0.35f, 1.0f });

			FontLibrary::Get().Push(VT_FONT_TYPE_BOLD);
			if (ImGui::Button("X", buttonSize)) values.x = resetValue;
			FontLibrary::Get().Pop();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth - buttonSize.x);
			ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.20f, 0.70f, 0.20f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.30f, 0.80f, 0.30f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.40f, 0.90f, 0.40f, 1.0f });

			FontLibrary::Get().Push(VT_FONT_TYPE_BOLD);
			if (ImGui::Button("Y", buttonSize)) values.y = resetValue;
			FontLibrary::Get().Pop();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth - buttonSize.x);
			ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, { 0.10f, 0.25f, 0.80f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.20f, 0.35f, 0.90f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.30f, 0.45f, 1.00f, 1.0f });

			FontLibrary::Get().Push(VT_FONT_TYPE_BOLD);
			if (ImGui::Button("Z", buttonSize)) values.z = resetValue;
			FontLibrary::Get().Pop();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			ImGui::PushItemWidth(itemWidth - buttonSize.x);
			ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();

			ImGui::PopStyleVar();
			ImGui::Columns(1);
			ImGui::PopID();
		}
	}
}