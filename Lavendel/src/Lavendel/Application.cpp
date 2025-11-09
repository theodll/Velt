#include "Application.h"
#include "Renderer/Renderer.h"

#include <array>

// CONSTRUCTOR 
namespace Lavendel {



	Application::Application()
	{
		
	}

	Application::~Application()
	{
	
	}

	// PUBLIC METHODS

	void Lavendel::Application::Run()
	{
		// 1st and 2nd param are width and height, 3rd is title, 4th is resizable bool

		while (!m_Window.ShouldClose())
		{
			m_Window.PollEvents();
			m_Renderer->drawFrame();
		}
	}

}
