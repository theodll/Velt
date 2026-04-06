#pragma once
#include "Core/Core.h"
#include <imgui.h>

#include <map>

namespace Velt::Editor 
{
	enum FontType
	{
		VT_FONT_TYPE_REGULAR, 
		VT_FONT_TYPE_BOLD,
		VT_FONT_TYPE_ITALIC,
		VT_FONT_TYPE_BOLD_ITALIC,

		VT_FONT_TYPE_SPECIAL_REGULAR, 
		VT_FONT_TYPE_SPECIAL_BOLD,
		VT_FONT_TYPE_SPECIAL_LIGHT,
		VT_FONT_TYPE_SPECIAL_MEDIUM,
		VT_FONT_TYPE_SPECIAL_SEMI_BOLD
	};

	class FontLibrary {
	public:
		static FontLibrary& Get() {
			static FontLibrary instance;
			return instance;
		}

		FontLibrary(const FontLibrary&) = delete;
		FontLibrary& operator=(const FontLibrary&) = delete;

		void Initialize() {
			ImGuiIO& io = ImGui::GetIO();

			Fonts[VT_FONT_TYPE_REGULAR] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/segoe-ui/segoe-ui-regular.ttf", 16.0f);
			Fonts[VT_FONT_TYPE_BOLD] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/segoe-ui/segoe-ui-bold.ttf", 16.0f);
			Fonts[VT_FONT_TYPE_ITALIC] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/segoe-ui/segoe-ui-italic.ttf", 16.0f);
			Fonts[VT_FONT_TYPE_BOLD_ITALIC] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/segoe-ui/segoe-ui-bold-italic.ttf", 16.0f);

			Fonts[VT_FONT_TYPE_SPECIAL_REGULAR] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/space-grotesk/SpaceGrotesk-Regular.ttf", 16.0f);
			Fonts[VT_FONT_TYPE_SPECIAL_BOLD] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/space-grotesk/SpaceGrotesk-Bold.ttf", 16.0f);


			for (auto const& [key, val] : Fonts) {
				if (!val) Fonts[key] = io.Fonts->AddFontDefault();
			}
		}

		void Push(FontType type) { ImGui::PushFont(Fonts[type]); }
		void Pop() { ImGui::PopFont(); }

		ImFont* GetFont(FontType type) { return Fonts[type]; }

	private:
		FontLibrary() {}
		std::map<FontType, ImFont*> Fonts;
	};
}