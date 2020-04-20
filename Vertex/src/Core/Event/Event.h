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
	        try
	        {
		        m_Event.m_Handled = func(m_Event);
	        }
	        catch (const std::invalid_argument& e)
	        {
		        Logger::GetCoreLogger()->debug("Failed attempt to dispatch event at {0}: {1}", &m_Event, m_Event.GetEventName());
	        }
        }

    private:
        Event& m_Event;
    };

}