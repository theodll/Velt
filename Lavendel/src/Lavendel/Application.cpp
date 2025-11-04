#include "Application.h"
#include "Renderer/Window.h"

// CONSTRUCTOR 

Lavendel::Application::Application()
{
}

Lavendel::Application::~Application()
{
}

// PUBLIC METHODS

void Lavendel::Application::Run()
{

	Lavendel::Window window(1280, 720, "Lavendel Engine");
	
	while (!window.ShouldClose())
	{
		window.PollEvents();
	}
}

