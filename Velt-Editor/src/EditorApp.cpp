#include <Velt.h>
#include <ImGui/ImGuiLayer.h>

#include "EditorLayer/EditorLayer.h"

namespace Velt {



	class VeltEditor : public  Velt::Application
	{
	public:
		VeltEditor()
		{
			VT_PROFILE_FUNCTION();
			PushLayer(new EditorLayer());
		}


		~VeltEditor() { VT_PROFILE_FUNCTION(); }
	};

	Velt::Application* Velt::CreateApplication()
	{
		VT_PROFILE_FUNCTION();
		return new VeltEditor();
	}
}