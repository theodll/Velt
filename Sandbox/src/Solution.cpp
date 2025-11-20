#include <Lavendel.h>

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
}; 



class Sandbox : public  Lavendel::Application
{
public: 
		Sandbox() 
		{
			LV_PROFILE_FUNCTION();
			PushLayer(new ExampleLayer());
			PushLayer(new Lavendel::ImGuiLayer());
		}
		~Sandbox() { LV_PROFILE_FUNCTION(); }
};


Lavendel::Application* Lavendel::CreateApplication()
{
	LV_PROFILE_FUNCTION();
	return new Sandbox();
}