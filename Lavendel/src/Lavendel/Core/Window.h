#pragma once
#include "lvpch.h"
#include "../Core.h"
#include <SDL3/SDL.h>


namespace Lavendel {
    namespace RenderAPI {

        struct WindowProps
        {
			std::string title;
            uint16_t width;
            uint16_t height;
            bool bResizabel;
		
            WindowProps(const std::string& t = "Lavendel Engine", uint16_t w = 1280, uint16_t h = 720, bool resizable = false)
				: title(t), width(w), height(h), bResizabel(resizable) {
			}

        };
        
		class LAVENDEL_API Window
		{
		public:
			using EventCallbackFn = std::function<void(Event&)>;


			virtual ~Window() {};

			virtual void OnUpdate() = 0;

			virtual uint16_t GetWidth() const = 0;
			virtual uint16_t GetHeight() const = 0;

			virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
			virtual void SetVSync(bool enabled) = 0;
			virtual bool IsVSync() const = 0;

			static Window* Create(const WindowProps& props = WindowProps());
		}


   //     class LAVENDEL_API Window
   //     {
   //     public:

			//using EventCallBackFn = std::function<void(SDL_Event&)>;

			//static Window* Create(const WindowProps& props = WindowProps());
   //         virtual ~Window();

   //         Window(const Window&) = delete;
   //         Window& operator=(const Window&) = delete;


   //         virtual void OnUpdate() = 0;

   //         // Getters
			//virtual uint16_t GetWidth() const { return m_Width; }
			//virtual uint16_t GetHeight() const { return m_Height; }
   //         void* GetNativeHandle() const { return m_Window; }
   //         VkExtent2D getExtent() const { return { static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height) }; }



   //         void PollEvents() { SDL_PumpEvents(); }
   //         bool ShouldClose();
   //         void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
			//bool wasWindodwResized() const { return m_bFrameBufferResized; }
			//void resetWindowResizedFlag() { m_bFrameBufferResized = false; }

   //     private:
   //         void Init(int width, int height, const std::string& title, bool bResizable);
   //         void Shutdown();


   //         SDL_Window* m_Window = nullptr;
   //         int m_Width{};
   //         int m_Height{};
   //         bool m_bResizable = false;
			//bool m_bFrameBufferResized = false;
   //         std::string m_Title;
   //     };

    }
}
