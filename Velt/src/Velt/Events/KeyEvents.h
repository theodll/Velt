#pragma once
#include "Event.h"
#include <sstream>

namespace Velt {

    class KeyPressedEvent : public Event {
    public:
        KeyPressedEvent(int key, bool repeat) : Key(key), Repeat(repeat) {}

        int Key = 0;
        bool Repeat = false;

        std::string ToString() const override {
            std::stringstream ss;
            ss << GetName() << ": " << Key << " repeat=" << (Repeat ? "true" : "false");
            return ss.str();
        }

        VELT_EVENT_CLASS_TYPE(KeyPressed)
        VELT_EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
    };

    class KeyReleasedEvent : public Event {
    public:
        explicit KeyReleasedEvent(int key) : Key(key) {}

        int Key = 0;

        std::string ToString() const override {
            std::stringstream ss;
            ss << GetName() << ": " << Key;
            return ss.str();
        }

        VELT_EVENT_CLASS_TYPE(KeyReleased)
        VELT_EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
    };

} // namespace Velt
