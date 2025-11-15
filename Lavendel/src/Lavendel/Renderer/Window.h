#pragma once
#include "lvpch.h"
#include "../Core.h"
#include <SDL3/SDL.h>


namespace Lavendel {
    namespace RenderAPI {

        struct WindowState
        {
            int width;
            int height;
			bool bResizabel;
        };
        

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
			bool wasWindodwResized() const { return m_bFrameBufferResized; }
			void resetWindowResizedFlag() { m_bFrameBufferResized = false; }

        private:
            void Init(int width, int height, const std::string& title, bool bResizable);
            void Shutdown();


            SDL_Window* m_Window = nullptr;
            int m_Width{};
            int m_Height{};
            bool m_bResizable = false;
			bool m_bFrameBufferResized = false;
            std::string m_Title;
        };

    }
}
