#include <Lavendel.h>

class Sandbox : public Lavendel::Application
{
public: 
		Sandbox() {}
		~Sandbox() {}
};


Lavendel::Application* Lavendel::CreateApplication()
{
	return new Sandbox();
}