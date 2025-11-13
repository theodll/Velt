#include "Window.h"
#include "../Log.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <stdexcept>

namespace Lavendel {
    namespace RenderAPI {

        static bool s_SDLInitialized = false;

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

            if (!s_SDLInitialized)
            {
                if (!SDL_Init(SDL_INIT_VIDEO))
                {
                    LV_CORE_ERROR("Failed to initialize SDL: {}", SDL_GetError());
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
                LV_CORE_ERROR("Failed to create SDL window: {}", SDL_GetError());
                return;
            }
        }

        void Window::Shutdown()
        {
            if (m_Window)
            {
                SDL_DestroyWindow(m_Window);
                m_Window = nullptr;
            }
        }

        void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
        {
            if (!SDL_Vulkan_CreateSurface(m_Window, instance, nullptr, surface))
            {
                LV_CORE_ERROR("Failed to create Vulkan surface: {}", SDL_GetError());
                throw std::runtime_error("failed to create window surface!");
            }
        }

        bool Window::ShouldClose()
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                    return true;
            }
            return false;
        }

    }
}
