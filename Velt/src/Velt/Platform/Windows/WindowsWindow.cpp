#include "WindowsWindow.h"
#include "Core/Application.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

namespace Velt 
{
	Velt::Window* Velt::Window::Create(const WindowProps& props)
	{
		VT_PROFILE_FUNCTION();
		return new Windows::WindowsWindow(props);
	}
}


namespace Velt::Windows
{

	static bool s_SDLInitialized = false;

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		VT_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{	
		VT_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		VT_PROFILE_FUNCTION();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		VT_PROFILE_FUNCTION();

		m_Data.m_Title = props.m_Title;
		m_Data.m_Width = props.m_Width;
		m_Data.m_Height = props.m_Height;

		VT_CORE_INFO("Creating WindowsWindow {0} ({1} {2})", m_Data.m_Title, m_Data.m_Width, m_Data.m_Height);

		if (!s_SDLInitialized)
		{
			if (!SDL_Init(SDL_INIT_VIDEO))
			{
				VT_CORE_ERROR("Failed to initialize SDL: {}", SDL_GetError());
				return;
			}
			s_SDLInitialized = true;
		}

		u32 windowFlags = SDL_WINDOW_VULKAN;
		if (m_Data.m_bResizable)
			windowFlags |= SDL_WINDOW_RESIZABLE;

		m_Window = SDL_CreateWindow(m_Data.m_Title.c_str(), m_Data.m_Width, m_Data.m_Height, windowFlags);
		if (!m_Window)
		{
			VT_CORE_ERROR("Failed to create SDL window: {}", SDL_GetError());
			return;
		}

		SDL_PropertiesID sdlProps = SDL_GetWindowProperties(m_Window);
		SDL_SetPointerProperty(sdlProps, "WindowInstance", this);
	}

	void WindowsWindow::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		if (m_Window)
		{
			SDL_DestroyWindow(m_Window);
			m_Window = nullptr;
		}
	}

	void WindowsWindow::setEventCallback(const EventCallbackFn& callback)
	{
		VT_PROFILE_FUNCTION();
	}

	void WindowsWindow::setVsync(bool enable) const
	{
		VT_PROFILE_FUNCTION();
	
		m_Data.m_bVsync = enable;
	}

	void WindowsWindow::setResizable(bool enable) const
	{
		VT_PROFILE_FUNCTION();
	}

	void WindowsWindow::createWindowSurface(void* instance, void* surface)
	{
		VT_PROFILE_FUNCTION();

		switch (Renderer::Renderer::GetAPI())
		{
		case Renderer::RendererAPI::Vulkan:
			if (!SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(Aplication::Get.getWindow.GetNativeHandle()), static_cast<VkInstance>(instance), nullptr, static_cast<VkSurfaceKHR*>(surface)))
			{
				VT_CORE_ERROR("Failed to create window surface: {}", SDL_GetError());
			}
		}
	}

 

}