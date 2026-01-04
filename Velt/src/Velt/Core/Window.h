#pragma once
#include "Core/Core.h"
#include <string>
#include "Events/Event.h"

namespace Velt
{

	struct WindowProps
	{
		std::string m_Title;
		u32 m_Width; 
		u32 m_Height;

		WindowProps(const std::string& title = "Velt Engine",
					u32 width = 1280,
					u32 height = 720)
					: m_Title(title), m_Width(width), m_Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};
		
		virtual void OnUpdate() = 0; 

		virtual u32 GetWidth() const = 0; 
		virtual u32 GetHeight() const = 0;
		virtual bool IsVsync() const = 0; 

		// virtual void SetEventCallback(const EventCallbackFn& callback) = 0; 
		virtual void SetVsync(bool enable) = 0;
		virtual void SetResizable(bool enable) = 0; 
		virtual void* GetNativeHandle() const = 0;


		virtual void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) = 0;
		virtual void CreateSwapchain() = 0;

		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());

	};


}