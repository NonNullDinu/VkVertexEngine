#pragma once

#include "Core/Core.h"

#include "spdlog/fmt/fmt.h"

#define VX_GET_EVENT_TYPE_CHAR(x) #x

namespace Vertex {

    enum class EventTypes
    {
        WindowResize, WindowClose,
        KeyPress, KeyRelease,
        MouseClick, MouseRelease, MouseScroll, MouseMove
    };

    enum class EventCategories
    {
        WindowEvent,
        KeyEvent,
        MouseEvent
    };

    class Event
    {
        friend class EventHandler;

    public:
        virtual EventCategories GetEventCategory() = 0;
        virtual EventTypes GetEventType() = 0;
        virtual const char* GetEventName() = 0;
        virtual const char* GetDetails() = 0;
    
    protected:
        bool m_Handled;
    };

    class EventHandler
    {
    public:
        EventHandler(Event& event)
            : m_Event(event)
        {
        }

        template<typename T>
        void Dispatch(T&& func)
        {
            m_Event.m_Handled = func(m_Event);
        }

    private:
        Event& m_Event;
    };

}