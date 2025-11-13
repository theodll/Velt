#pragma once

#include "../Core.h"
#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>
#include <string>

namespace Lavendel {
    namespace RenderAPI {

        class LAVENDEL_API Window
        {
        public:
            Window(int width, int height, const std::string& title, bool bResizable = false);
            ~Window();

            Window(const Window&) = delete;
            Window& operator=(const Window&) = delete;

            void PollEvents() { SDL_PumpEvents(); }
            bool ShouldClose();
            void* GetNativeHandle() const { return m_Window; }
            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
            VkExtent2D getExtent() const { return { static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height) }; }

        private:
            void Init(int width, int height, const std::string& title, bool bResizable);
            void Shutdown();

            SDL_Window* m_Window = nullptr;
            int m_Width = 0;
            int m_Height = 0;
            bool m_Resizable = false;
            std::string m_Title;
        };

    }
}
