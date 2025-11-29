#include <Lavendel.h>
#include <ImGui/ImGuiLayer.h>

class ExampleLayer : public Lavendel::Layer
{
	public:
	ExampleLayer() : Layer("Example") { LV_PROFILE_FUNCTION(); }
	void OnUpdate() override
	{
		LV_PROFILE_FUNCTION();
		//LV_CORE_INFO("ExampleLayer::Update");
	}
	void OnEvent(Lavendel::Event& event) override
	{
		LV_PROFILE_FUNCTION();
		LV_CORE_INFO("{0}", event.ToString());
	}

	void OnImGuiRender() 
	{
		LV_PROFILE_FUNCTION();
		//LV_CORE_INFO("ExampleLayer::OnRender");

		ImGui::Begin("Hello from ExampleLayer");

	}
}; 



class Sandbox : public  Lavendel::Application
{
public: 
		Sandbox() 
		{
			LV_PROFILE_FUNCTION();
			PushLayer(new ExampleLayer());
			PushOverlay(new Lavendel::ImGuiLayer());

		}


		~Sandbox() { LV_PROFILE_FUNCTION(); }
};


Lavendel::Application* Lavendel::CreateApplication()
{
	LV_PROFILE_FUNCTION();
	return new Sandbox();
}