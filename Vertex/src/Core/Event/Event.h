#pragma once

#include "Core/Core.h"

#include "spdlog/fmt/fmt.h"

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

    class VX_PUBLIC_API Event
    {
        friend class EventHandler;

    public:
        virtual EventCategories GetEventCategory() = 0;
        virtual EventTypes GetEventType() = 0;
        virtual const char* GetEventName() = 0;
        virtual std::string GetDetails() = 0;

        inline bool IsHandled() const { return m_Handled; }

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

        template<typename T, typename F>
        void Dispatch(F&& func)
        {
            m_Event.m_Handled = func((T&)(m_Event));
        }

    private:
        Event& m_Event;
    };

}