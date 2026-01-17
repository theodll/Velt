#include <Velt.h>
#include "../Velt/Renderer/Renderer.h"

class EditorLayer : public Velt::Layer
{
public:
	EditorLayer() : Layer("Editor") { VT_PROFILE_FUNCTION(); }


	void OnUpdate() override
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("ExampleLayer::Update");
	}
	void OnEvent(Velt::Event& event) override
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("{0}", event.ToString());
	}

	void OnRender(VkCommandBuffer commandBuffer) 
	{
		Velt::Renderer::Renderer::DrawQuad(commandBuffer);
	}

	void OnImGuiRender()
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("ExampleLayer::OnRender");

		// ImGui::Begin("Hello from ExampleLayer");

	}
};