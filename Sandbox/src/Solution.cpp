#include <Velt.h>
#include <ImGui/ImGuiLayer.h>

class ExampleLayer : public Velt::Layer
{
	public:
	ExampleLayer() : Layer("Example") { VT_PROFILE_FUNCTION(); }
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

	void OnImGuiRender() 
	{
		VT_PROFILE_FUNCTION();
		//VT_CORE_INFO("ExampleLayer::OnRender");

//		ImGui::Begin("Hello from ExampleLayer");

	}
}; 



class Sandbox : public  Velt::Application
{
public: 
		Sandbox() 
		{
			VT_PROFILE_FUNCTION();
			PushLayer(new ExampleLayer());
		//	PushOverlay(new Velt::ImGuiLayer());

		}


		~Sandbox() { VT_PROFILE_FUNCTION(); }
};


Velt::Application* Velt::CreateApplication()
{
	VT_PROFILE_FUNCTION();
	return new Sandbox();
}