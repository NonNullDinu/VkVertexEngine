#include "Core.h"

#define VX_GET_EVENT_TYPE_CHAR(x) ##x

namespace Vertex {

    enum class EventTypes
    {
        KeyPressEvent, KeyReleaseEvent,
        MouseClickEvent, MouseReleaseEvent, MouseScrollEvent, MouseMoveEvent
    };

    class Event
    {
        friend class EventDispatcher;

    public:
        virtual EventTypes  GetEventType();
        virtual const char* GetEventName();
        virtual const char* GetDetails();
    
    protected:
        bool m_Handled;
    };

    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : m_Event(event)
        {
        }

        template<EventTypes T>
        void Dispatch(bool(func*)(Event))
        {
            if (T == m_Event.GetEventType())
            {
                m_Event.m_Handled = func(m_Event);
            }
        }

    private:
        Event& m_Event
    };

}