#pragma once
#include "Core/Core.h"
#include "FontAwesomeIcons.h"
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
		VT_FONT_TYPE_SPECIAL_SEMI_BOLD, 

		VT_FONT_TYPE_ICON,

		VT_FONT_TYPE_SIZE
	};

	class FontLibrary {
	public:
		static FontLibrary& Get() {
			static FontLibrary instance;
			return instance;
		}

		FontLibrary(const FontLibrary&) = delete;
		FontLibrary& operator=(const FontLibrary&) = delete;

		void Init();
		void Shutdown();

		void Push(FontType type) { ImGui::PushFont(Fonts[type]); }
		void Pop() { ImGui::PopFont(); }

		ImFont* GetFont(FontType type) { return Fonts[type]; }

	private:
		FontLibrary() {}
		std::map<FontType, ImFont*> Fonts;
	};
}