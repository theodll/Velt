#pragma once

#include "imgui.h"
#include <string>

namespace Velt {
	class Velt_API DemoWidget {
	public:
		DemoWidget(const std::string& name = "Demo Widget");
		~DemoWidget() = default;

		void OnRender();
		void SetVisible(bool visible) { m_IsVisible = visible; }
		bool IsVisible() const { return m_IsVisible; }

	private:
		std::string m_Name;
		bool m_IsVisible = true;
		float m_FloatValue = 0.5f;
		int m_IntValue = 42;
		bool m_Boovtalue = true;
		char m_TextBuffer[256] = "Enter text here...";
	};
}
