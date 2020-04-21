#pragma once

#include "Core/Core.h"

#include "spdlog/fmt/fmt.h"

namespace Vertex {

    enum class EventTypes
    {
        WindowResize, WindowClose, WindowGainedFocus, WindowLostFocus, WindowCursorEntered, WindowCursorLeft,
        KeyPress, KeyRelease,
        MouseClick, MouseRelease, MouseScroll, MouseMove
    };

    enum class EventCategories
    {
        WindowEvent,
        KeyEvent,
        MouseEvent
    };

    class VX_PUBLIC_API Event
    {
        friend class EventHandler;

    public:
        virtual EventCategories GetEventCategory() = 0;
        virtual EventTypes GetEventType() = 0;
        virtual const char* GetEventName() = 0;
        virtual std::string GetDetails() = 0;

        inline bool IsHandled() { return m_Handled; }

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

        template<auto T, typename F>
        void Dispatch(F&& func)
        {
            if (m_Event.GetEventType() == T && !m_Event.m_Handled)
            {
                m_Event.m_Handled = func(m_Event);
            }
        }

    private:
        Event& m_Event;
    };

}