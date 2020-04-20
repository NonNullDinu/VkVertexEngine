#pragma once

#include "Event.h"

namespace Vertex {

    class WindowEvent : public Event {
    public:
        inline EventCategories GetEventCategory() override { return EventCategories::WindowEvent; }
    };

    class WindowResizeEvent : public WindowEvent {
    public:
        WindowResizeEvent(unsigned int width,
                          unsigned int height)
                : m_Width(width), m_Height(height)
        {
        }

        inline unsigned int GetWidth() { return m_Width; }

        inline unsigned int GetHeight() { return m_Height; }

        inline EventTypes GetEventType() override { return EventTypes::WindowResize; }

        inline const char *GetEventName() override { return "EventTypes::WindowResize"; }

        const char *GetDetails() override
        {
            return fmt::format("WindowResizeEvent : Width<{0}> Height<{1}>", m_Width, m_Height).c_str();
        }

    private:
        unsigned int m_Width;
        unsigned int m_Height;
    };

    class WindowCloseEvent : public WindowEvent {
    public:
        WindowCloseEvent()
        {
        }

        inline EventTypes GetEventType() override { return EventTypes::WindowClose; }

        inline const char *GetEventName() override { return "EventTypes::WindowClose"; }

        const char *GetDetails() override
        {
            return "WindowCloseEvent";
        }
    };

}