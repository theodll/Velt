#pragma once
#include "Event.h"
#include <SDL3/SDL.h>

namespace Velt
{
    class EventHandler
    {
    public:
        virtual ~EventHandler() = default;

        static void ProcessEvent(Event& event);

    };

    class VELT_API EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;

    public:
        EventDispatcher(Event& event) : m_Event(event)
        {
        }

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.IsHandled = func(*(T*)&m_Event);
                return true;
            }
            return false;
        }
    private:
        Event& m_Event;
    };
    /*
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }*/
}
