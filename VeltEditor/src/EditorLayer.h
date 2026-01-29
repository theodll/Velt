#include <Velt.h>
#include "../Velt/Renderer/Renderer.h"

namespace Editor {

	class EditorLayer : public Velt::Layer
	{
	public:
		EditorLayer();


		void OnUpdate(Velt::Timestep ts) override;
		void OnEvent(Velt::Event& event) override;
		void OnRender(VkCommandBuffer commandBuffer) override;

		void OnImGuiRender();
	};
}