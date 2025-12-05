#pragma once
#include "vtpch.h"
#include "Event.h"


namespace Velt
{
    class Velt_API MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}
        inline float GetX() const { return m_MouseX; }
        inline float GetY() const { return m_MouseY; }
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_MouseX, m_MouseY;
    };

    class Velt_API MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float OffsetX, float OffsetY) : m_OffsetX(OffsetX), m_OffsetY(OffsetY) {};

        inline float GetOffsetX() { return m_OffsetX; }
        inline float GetOffsetY() { return m_OffsetY; }

        std::string ToString()
        {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << m_OffsetX << ", " << m_OffsetY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_OffsetX, m_OffsetY;
    };

    class Velt_API MouseButtonEvent : public Event
    {
        inline int GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    protected:

        int m_Button;
    };

    class Velt_API MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button) : m_Button(button) {};

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    private:
        int m_Button;
    };

    class Velt_API MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button) : m_Button(button) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << m_Button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)

    private:
        int m_Button;
    };
}