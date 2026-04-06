#include "vtpch.h"
#include "Core/Core.h"
#include "FontLibrary.h"

namespace Velt::Editor 
{

	void FontLibrary::Init()
	{
		ImGuiIO& io = ImGui::GetIO();

		Fonts[VT_FONT_TYPE_REGULAR] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/segoe-ui/segoe-ui-regular.ttf", 16.0f);
		Fonts[VT_FONT_TYPE_BOLD] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/segoe-ui/segoe-ui-bold.ttf", 16.0f);
		Fonts[VT_FONT_TYPE_ITALIC] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/segoe-ui/segoe-ui-italic.ttf", 16.0f);
		Fonts[VT_FONT_TYPE_BOLD_ITALIC] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/segoe-ui/segoe-ui-bold-italic.ttf", 16.0f);

		Fonts[VT_FONT_TYPE_SPECIAL_REGULAR] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/space-grotesk/SpaceGrotesk-Regular.ttf", 16.0f);
		Fonts[VT_FONT_TYPE_SPECIAL_BOLD] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/space-grotesk/SpaceGrotesk-Bold.ttf", 16.0f);
		Fonts[VT_FONT_TYPE_SPECIAL_LIGHT] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/space-grotesk/SpaceGrotesk-Light.ttf", 16.0f);
		Fonts[VT_FONT_TYPE_SPECIAL_MEDIUM] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/space-grotesk/SpaceGrotesk-Medium.ttf", 16.0f);
		Fonts[VT_FONT_TYPE_SPECIAL_SEMI_BOLD] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/space-grotesk/SpaceGrotesk-SemiBold.ttf", 16.0f);

		Fonts[VT_FONT_TYPE_ICON] = io.Fonts->AddFontFromFileTTF("Assets/Fonts/font-awesome/font-awesome-regular.ttf", 16.0f);

		for (auto const& [key, val] : Fonts) {
			if (!val) Fonts[key] = io.Fonts->AddFontDefault();
		}
		
	}

	void FontLibrary::Shutdown()
	{

	}

}