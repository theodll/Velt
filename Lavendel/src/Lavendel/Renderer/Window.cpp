#include "Window.h"
#include "Setup/Device.h"
#include "Setup/Pipeline.h"
#include "../Log.h"

namespace Lavendel {
	namespace RendererAPI {

		static bool s_GLFWInitialized = false;

		Window::Window(int width, int height, const std::string& title, bool bResizable) 
			: m_Width(width), m_Height(height), m_Title(title), m_Resizable(bResizable)
		{
			Init(width, height, title, bResizable);
		}

		Window::~Window()
		{
			Shutdown();
		}

		void Window::Init(int width, int height, const std::string& title, bool bResizable)
		{
			m_Width = width;
			m_Height = height;
			m_Title = title;
			m_Resizable = bResizable;

			if (!s_GLFWInitialized)
			{
				if (!glfwInit())
				{
					LV_CORE_ERROR("Failed to initialize GLFW!");
					return;
				}

				s_GLFWInitialized = true;
			}

			if (!bResizable)
			{
				glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
			}

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

			if (!m_Window)
			{
				LV_CORE_ERROR("Failed to create GLFW window!");
				return;
			}

			// Initialize GPUDevice and Pipeline after m_Window is created
			m_Device = std::make_unique<GPUDevice>(*this);
			m_Pipeline = std::make_unique<Pipeline>(*m_Device, 
													"shaders/shader.vert.spv", 
													"shaders/shader.frag.spv", 
													Pipeline::defaultPipelineConfigInfo(m_Width, m_Height));
		}

		void Window::Shutdown()
		{
			// Destroy in reverse order
			m_Pipeline.reset();
			m_Device.reset();
			
			if (m_Window)
			{
				glfwDestroyWindow(m_Window);
			}
			
			glfwTerminate();
		}

		void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
		{
			if (glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS)
			{
				LV_CORE_ERROR("Failed to create window surface!");
				throw std::runtime_error("failed to create window surface!");
			}
		}

	}
}