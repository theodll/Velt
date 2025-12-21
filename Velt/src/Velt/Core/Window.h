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

	class VELT_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};
		
		virtual void OnUpdate() = 0; 

		virtual u32 getWidth() const = 0; 
		virtual u32 getHeight() const = 0;
		virtual bool isVsync() const = 0; 

		virtual void setEventCallback(const EventCallbackFn& callback) = 0; 
		virtual void setVsync(bool enable) const = 0;
		virtual void setResizable(bool enable) const = 0; 

		static Window* Create(const WindowProps& props = WindowProps());

	};


}