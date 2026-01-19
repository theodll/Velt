#include "WindowsWindow.h"
#include "Core/Application.h"
#include <SDL3/SDL.h>
#include "Core/Window.h"
#include <SDL3/SDL_vulkan.h>
#include "Velt/Renderer/Renderer.h"
#include "Velt/Platform/Vulkan/VulkanSwapchain.h"
#include "Velt/Platform/Vulkan/VulkanContext.h"
#include "Renderer/RenderAPI.h"
#include "Renderer/RenderContext.h"
#include <vulkan/vulkan.h>


namespace Velt 
{
	std::unique_ptr<Window> Velt::Window::Create(const WindowProps& props)
	{
		VT_PROFILE_FUNCTION();
		return std::make_unique<Windows::WindowsWindow>(props);
	}
}


namespace Velt::Windows
{

	void* WindowsWindow::GetNativeHandle() const
	{
		return m_Window;
	}

	static bool s_SDLInitialized = false;

	WindowsWindow::WindowsWindow(const WindowProps& props) : m_WindowProps(props)
	{
		VT_PROFILE_FUNCTION();
		Init();
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

	void WindowsWindow::Init()
	{
		VT_PROFILE_FUNCTION();

		m_Data.Title = m_WindowProps.m_Title;
		m_Data.Width = m_WindowProps.m_Width;
		m_Data.Height = m_WindowProps.m_Height;

		VT_CORE_INFO("Creating WindowsWindow {0} ({1} {2})", m_Data.Title, m_Data.Width, m_Data.Height);

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
		if (m_Data.bResizable)
			windowFlags |= SDL_WINDOW_RESIZABLE;

		m_Window = SDL_CreateWindow(m_Data.Title.c_str(), m_Data.Width, m_Data.Height, windowFlags);
		if (!m_Window)
		{
			VT_CORE_ERROR("Failed to create SDL window: {}", SDL_GetError());
			return;
		}

		SDL_PropertiesID sdlProps = SDL_GetWindowProperties(m_Window);
		SDL_SetPointerProperty(sdlProps, "WindowInstance", this);

	//	m_Context = Renderer::Context::Create();
	//	m_Context->Init();
	//	CreateWindowSurface(Renderer::Vulkan::VulkanContext::GetInstance(), &Renderer::Vulkan::VulkanContext::GetSurface());


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

	//void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	//{
	//	VT_PROFILE_FUNCTION();
	//}

	void WindowsWindow::SetVsync(bool enable)
	{
		VT_PROFILE_FUNCTION();
	
		m_Data.bVsync = enable;
	}

	void WindowsWindow::SetResizable(bool enable)
	{
		VT_PROFILE_FUNCTION();
		m_Data.bResizable = enable;
	}

	void WindowsWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		VT_PROFILE_FUNCTION();

		switch (Renderer::Renderer::GetAPI())
		{
		case Renderer::RenderAPI::API::Vulkan:
			if (!SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeHandle()), instance, nullptr, surface))
			{
				VT_CORE_ERROR("Failed to create window surface: {}", SDL_GetError());
			}
		}
	}

 
	void WindowsWindow::CreateSwapchain() 
	{
		m_Swapchain = std::make_unique<Renderer::Vulkan::VulkanSwapchain>();

		Renderer::Vulkan::SwapchainCreateInfo createInfo{};

		createInfo.Height = m_Data.Height;
		createInfo.Width = m_Data.Width;
		createInfo.VSync =  m_Data.bVsync;
		
		m_Swapchain->Init(createInfo);
		m_Swapchain->InitSurface(m_Window);
	}


	Renderer::Vulkan::VulkanSwapchain& WindowsWindow::GetSwapchain()
	{
		return *m_Swapchain;
	}

}