#include "EditorLayer.h"

namespace Editor
{

	EditorLayer::EditorLayer() : Layer("Editor")
	{
		VT_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Velt::Timestep ts)
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("ExampleLayer::Update");


	}

	void EditorLayer::OnEvent(Velt::Event& event)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("{0}", event.ToString());
	}

	void EditorLayer::OnRender(VkCommandBuffer commandBuffer)
	{
		Velt::Renderer::Renderer::DrawQuad(commandBuffer);
	}

	void EditorLayer::OnImGuiRender()
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("ExampleLayer::OnRender");

		// ImGui::Begin("Hello from ExampleLayer");

	}

}