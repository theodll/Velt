#include "Window.h"
#include "../Log.h"

namespace Lavendel {

	static bool s_GLFWInitialized = false;

	Window::Window(int width, int height, const std::string& title) : m_Width(width), m_Height(height), m_Title(title) 
	{
		Init(width, height, title);
	}

	Window::~Window()
	{
		Shutdown();
	};

	// TODO: Logging

	void Window::Init(int width, int height, const std::string& title)
	{
		if (!s_GLFWInitialized)
		{
			glfwSetErrorCallback([](int code, const char* desc)
								 {
									 fprintf(stderr, "GLFW ERROR [%d]: %s\n", code, desc);
								 });

			if (!glfwInit())
			{
				LV_CORE_ERROR("Failed to initialize GLFW!");
				return;
			}

			s_GLFWInitialized = true;
		}

		// Sag GLFW, dass du keinen OpenGL-Context willst
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (!m_Window)
		{
			LV_CORE_ERROR("Failed to create GLFW window!");
			return;
		}

		LV_CORE_INFO("Created Vulkan-compatible window!");
	}

	void Window::Shutdown() 
	{
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}

		glfwTerminate();
		s_GLFWInitialized = false;
		LV_CORE_INFO("GLFW terminated");


	}



}