#include <Lavendel.h>
#include <ImGui/ImGuiLayer.h>

#include "EditorLayer/EditorLayer.h"

namespace Lavendel {



	class LavendelEditor : public  Lavendel::Application
	{
	public:
		LavendelEditor()
		{
			LV_PROFILE_FUNCTION();
			PushLayer(new EditorLayer());
		}


		~LavendelEditor() { LV_PROFILE_FUNCTION(); }
	};

	Lavendel::Application* Lavendel::CreateApplication()
	{
		LV_PROFILE_FUNCTION();
		return new LavendelEditor();
	}
}