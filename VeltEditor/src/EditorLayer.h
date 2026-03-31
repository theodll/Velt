#pragma once
#include <Velt/Velt.h>

namespace Velt::Editor {

	class EditorLayer : public Velt::Layer
	{
	public:
		EditorLayer();

		void Init() override;
		void Shutdown() override;

		void OnAttach() override;
		void OnDetach() override; 
		void OnUpdate(Velt::Timestep ts) override;
		void OnEvent(Velt::Event& event) override;
		void OnRender(VkCommandBuffer commandBuffer) override;
		void OnImGuiRender2() override;
	private: 

		Ref<Scene> m_ActiveScene;

		Ref<Velt::Model> m_Model;
		Ref<Velt::Mesh> m_Mesh;
	};
}