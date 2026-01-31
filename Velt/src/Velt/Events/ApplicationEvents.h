#pragma once
#include "Event.h"
#include <sstream>

namespace Velt {

    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;

        VELT_EVENT_CLASS_TYPE(WindowClose)
        VELT_EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(int w, int h) : Width(w), Height(h) {}

        int Width = 0, Height = 0;

        std::string ToString() const override {
            std::stringstream ss;
            ss << GetName() << ": " << Width << ", " << Height;
            return ss.str();
        }

        VELT_EVENT_CLASS_TYPE(WindowResize)
        VELT_EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

} // namespace Velt
