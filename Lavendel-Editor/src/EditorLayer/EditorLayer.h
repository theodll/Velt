#include <Lavendel/Lavendel.h>

namespace Lavendel {

	class EditorLayer : public Lavendel::Layer
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

