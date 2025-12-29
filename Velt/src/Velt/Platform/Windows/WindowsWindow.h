#pragma once
#include "Core/Window.h"
#include "Core/Application.h"
#include <SDL3/SDL.h>

namespace Velt::Windows 
{
	class VELT_API WindowsWindow : public Window 
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline u32 getWidth() const override { return m_Data.m_Width; };
		inline u32 getHeight() const override { return m_Data.m_Height; };
		bool isVsync() const override { return m_Data.m_bVsync; };

		void setEventCallback(const EventCallbackFn& callback) override;
		void setVsync(bool enable) override;
		void setResizable(bool enable) override;
		void* GetNativeHandle() const override;

		void CreateWindowSurface(void* instance, void* surface);
		VkExtent2D getExtent() const {	return { Application::Get().GetWindow().getWidth(), Application::Get().GetWindow().getHeight() };	}
	private:
		SDL_Window* m_Window = nullptr;
		
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		struct WindowData 
		{
			std::string m_Title;
			u32 m_Width, m_Height;
			bool m_bVsync;
			bool m_bResizable; 

			EventCallbackFn EventCallback; 
		};

		WindowData m_Data; 

	};

}
