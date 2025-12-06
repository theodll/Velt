#include <Velt/Velt.h>
#include <ImGui/ImGuiLayer.h>
#include "EditorLayer.h"

class Editor : public Velt::Application
{
public:
	Editor()
	{
		VT_PROFILE_FUNCTION();
		PushLayer(new EditorLayer());
	}

	~Editor() { VT_PROFILE_FUNCTION(); }
};

Velt::Application *Velt::CreateApplication()
{
	VT_PROFILE_FUNCTION();
	return new Editor();
}