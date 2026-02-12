#pragma once
#include <Velt/Core/Core.h>
#include <Velt/Layers/Layer.h>
#include <Velt/Core/Timestep.h>
#include <Velt/Renderer/Renderer.h>

namespace Editor {

	class EditorLayer : public Velt::Layer
	{
	public:
		EditorLayer();

		void Init() override;
		void OnUpdate(Velt::Timestep ts) override;
		void OnEvent(Velt::Event& event) override;
		void OnRender(VkCommandBuffer commandBuffer) override;

		void OnImGuiRender2() override;
	private: 
		glm::vec3 m_CameraPos{};
		float m_CameraRot{};

		Velt::Ref<Velt::Model> m_Cube;
		int index{1};

		glm::vec3 m_Rotation{};
		glm::vec3 m_Scale{};
		glm::vec3 m_Translation{};

		glm::vec3 m_SquarePos{};
	};
}