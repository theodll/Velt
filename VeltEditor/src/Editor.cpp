#include <Velt/Velt.h>
#include <ImGui/ImGuiLayer.h>
#include "EditorLayer.h"


class EditorApp : public Velt::Application
{
public: 
	EditorApp()
	{
		VT_PROFILE_FUNCTION();
		
		PushLayer(new Editor::EditorLayer());
	
#if NDEBUG
		Application::UpdateTitle("Velt Editor - Release");
#else
		Application::UpdateTitle("Velt Editor - Debug");
#endif

	}


	~EditorApp() { VT_PROFILE_FUNCTION(); }
};


Velt::Application* Velt::CreateApplication()
{
	VT_PROFILE_FUNCTION();
	return new EditorApp();
}