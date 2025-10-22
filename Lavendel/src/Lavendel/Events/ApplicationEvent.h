#pragma once


#include "Event.h"

#include "sstream"

namespace Lavendel {
	class LAVENDEL_API WindowResizeEvent : public Event
	{

		public:
			WindowResizeEvent(uint16_t width, uint16_t height) : m_Width(width), m_Height(height){}

			inline uint16_t GetWidth() const { return m_Width; }
			inline uint16_t GetHeight() const { return m_Height; }

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "WindowsResizeEvent: " << m_Width << ", " << m_Height;
				return ss.str();
			}

			
			EVENT_CLASS_TYPE(WindowResize);
			EVENT_CLASS_CATEGORY(EventCategoryApplication);
	
	private:
		uint16_t m_Width, m_Height;

	};

	class LAVENDEL_API WindowCloseEvent : public Event
	{
		public:
			WindowCloseEvent() {};

			EVENT_CLASS_TYPE(WindowClose);
			EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class LAVENDEL_API AppTickEvent : public Event
	{
		public:
			AppTickEvent() {};
		
			EVENT_CLASS_TYPE(AppTick);
			EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class LAVENDEL_API AppUpdateEvent : public Event
	{
		public:
			AppUpdateEvent() {};
		
			EVENT_CLASS_TYPE(AppUpdate);
			EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};

	class LAVENDEL_API AppRenderEvent : public Event
	{
		public:
			AppRenderEvent() {};
		
			EVENT_CLASS_TYPE(AppRender);
			EVENT_CLASS_CATEGORY(EventCategoryApplication);
	};
}