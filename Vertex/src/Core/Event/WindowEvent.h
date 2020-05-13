#pragma once

#include "Event.h"

namespace Vertex
{

    class WindowEvent : public Event
    {
    public:
        inline EventCategories GetEventCategory() override { return EventCategories::WindowEvent; }
    };

    class WindowResizeEvent : public WindowEvent
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) { }

        inline unsigned int GetWidth() { return m_Width; }
        inline unsigned int GetHeight() { return m_Height; }

        inline EventTypes  GetEventType() override { return EventTypes::WindowResize; }
        inline const char* GetEventName() override { return "EventTypes::WindowResize"; }

        inline std::string GetDetails() override
        {
            return fmt::format("WindowResizeEvent : Width<{0}> Height<{1}>", m_Width, m_Height);
        }

    private:
        unsigned int m_Width, m_Height;
    };

    class WindowCloseEvent : public WindowEvent
    {
    public:
        WindowCloseEvent() { }

        inline EventTypes  GetEventType() override { return EventTypes::WindowClose; }
        inline const char* GetEventName() override { return "EventTypes::WindowClose"; }

        inline std::string GetDetails() override { return "WindowCloseEvent"; }
    };

    class WindowGainedFocusEvent : public WindowEvent
    {
    public:
        WindowGainedFocusEvent() { }

        inline EventTypes  GetEventType() override { return EventTypes::WindowGainedFocus; }
        inline const char* GetEventName() override { return "EventTypes::WindowGainedFocus"; }

        inline std::string GetDetails() override { return "WindowGainedFocusEvent"; }
    };

    class WindowLostFocusEvent : public WindowEvent
    {
    public:
        WindowLostFocusEvent() { }

        inline EventTypes  GetEventType() override { return EventTypes::WindowLostFocus; }
        inline const char* GetEventName() override { return "EventTypes::WindowLostFocus"; }

        inline std::string GetDetails() override { return "WindowLostFocusEvent"; }
    };

    class WindowCursorEnteredEvent : public WindowEvent
    {
    public:
        WindowCursorEnteredEvent() { }

        inline EventTypes  GetEventType() override { return EventTypes::WindowCursorEntered; }
        inline const char* GetEventName() override { return "EventTypes::WindowCursorEntered"; }

        inline std::string GetDetails() override { return "WindowCursorEnteredEvent"; }
    };

    class WindowCursorLeftEvent : public WindowEvent
    {
    public:
        WindowCursorLeftEvent() { }

        inline EventTypes  GetEventType() override { return EventTypes::WindowCursorLeft; }
        inline const char* GetEventName() override { return "EventTypes::WindowCursorLeft"; }

        inline std::string GetDetails() override { return "WindowCursorLeftEvent"; }
    };
}