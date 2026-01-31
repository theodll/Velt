#pragma once
#include "Event.h"
#include <sstream>

namespace Velt {

    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y) : X(x), Y(y) {}

        float X = 0, Y = 0;

        std::string ToString() const override {
            std::stringstream ss;
            ss << GetName() << ": " << X << ", " << Y;
            return ss.str();
        }

        VELT_EVENT_CLASS_TYPE(MouseMoved)
        VELT_EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
    };

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float xoff, float yoff) : XOffset(xoff), YOffset(yoff) {}

        float XOffset = 0, YOffset = 0;

        VELT_EVENT_CLASS_TYPE(MouseScrolled)
        VELT_EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
    };

    class MouseButtonPressedEvent : public Event {
    public:
        explicit MouseButtonPressedEvent(int button) : Button(button) {}

        int Button = 0;

        VELT_EVENT_CLASS_TYPE(MouseButtonPressed)
        VELT_EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)
    };

    class MouseButtonReleasedEvent : public Event {
    public:
        explicit MouseButtonReleasedEvent(int button) : Button(button) {}

        int Button = 0;

        VELT_EVENT_CLASS_TYPE(MouseButtonReleased)
        VELT_EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)
    };

} // namespace Velt
