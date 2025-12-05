#include <Velt/Velt.h>

namespace Velt {

	class EditorLayer : public Velt::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;

	};

}

