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


		void OnUpdate(Velt::Timestep ts) override;
		void OnEvent(Velt::Event& event) override;
		void OnRender(VkCommandBuffer commandBuffer) override;

		void OnImGuiRender() override;
	private: 
		glm::vec3 m_CameraPos{};
		float m_CameraRot{};

		glm::vec3 m_SquarePos{};
	};
}