#include "vtpch.h"

#include "Window.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

namespace Velt
{
    namespace RenderAPI
    {

        static bool s_SDLInitialized = false;

        Window::Window(int width, int height, const std::string &title, bool bResizable)
            : m_Width(width), m_Height(height), m_Title(title), m_bResizable(bResizable)
        {
            VT_PROFILE_FUNCTION();
            VT_CORE_INFO("Creating Window...");
            Init(width, height, title, bResizable);
        }

        Window::~Window()
        {
            VT_PROFILE_FUNCTION();
            Shutdown();
        }

        void Window::Init(int width, int height, const std::string &title, bool bResizable)
        {
            VT_PROFILE_FUNCTION();
            m_Width = width;
            m_Height = height;
            m_Title = title;
            m_bResizable = bResizable;

            if (!s_SDLInitialized)
            {
                if (!SDL_Init(SDL_INIT_VIDEO))
                {
                    VT_CORE_ERROR("Failed to initialize SDL: {}", SDL_GetError());
                    return;
                }
                s_SDLInitialized = true;
            }

            Uint32 windowFlags = SDL_WINDOW_VULKAN;
            if (bResizable)
                windowFlags |= SDL_WINDOW_RESIZABLE;

            m_Window = SDL_CreateWindow(title.c_str(), width, height, windowFlags);
            if (!m_Window)
            {
                VT_CORE_ERROR("Failed to create SDL window: {}", SDL_GetError());
                return;
            }

            // Store the Window instance pointer for later retrieval using SDL3 properties
            SDL_PropertiesID props = SDL_GetWindowProperties(m_Window);
            SDL_SetPointerProperty(props, "WindowInstance", this);
        }

        void Window::Shutdown()
        {
            VT_PROFILE_FUNCTION();
            if (m_Window)
            {
                SDL_DestroyWindow(m_Window);
                m_Window = nullptr;
            }
        }

        void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
        {
            VT_PROFILE_FUNCTION();
            if (!SDL_Vulkan_CreateSurface(m_Window, instance, nullptr, surface))
            {
                VT_CORE_ERROR("Failed to create Vulkan surface: {}", SDL_GetError());
                throw std::runtime_error("failed to create window surface!");
            }
        }

        bool Window::ShouldClose()
        {
            VT_PROFILE_FUNCTION();
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                VT_PROFILE_SCOPE("Window::ShouldClose PollEvent Loop");
                if (event.type == SDL_EVENT_QUIT)
                    return true;

                if (event.type == SDL_EVENT_WINDOW_RESIZED)
                {
                    m_bFrameBufferResized = true;
                    m_Width = event.window.data1;
                    m_Height = event.window.data2;
                }

                if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
                {
                    m_bFrameBufferResized = true;
                    m_Width = event.window.data1;
                    m_Height = event.window.data2;
                }
            }
            return false;
        }

    }
}
