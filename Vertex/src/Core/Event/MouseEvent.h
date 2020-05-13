#pragma once

#include "Event.h"

namespace Vertex
{

    class MouseEvent : public Event
    {
    public:
        inline EventCategories GetEventCategory() override { return EventCategories::MouseEvent; }
    };

    class MouseClickEvent : public MouseEvent
    {
    public:
        MouseClickEvent(int button) : m_Button(button) { }

        inline int GetButton() { return m_Button; }

        inline EventTypes  GetEventType() override { return EventTypes::MouseClick; }
        inline const char* GetEventName() override { return "EventTypes::MouseClick"; }

        inline std::string GetDetails() override { return fmt::format("MouseClickEvent : Button<{0}>", m_Button); }

    private:
        int m_Button;
    };

    class MouseReleaseEvent : public MouseEvent
    {
    public:
        MouseReleaseEvent(int button) : m_Button(button) { }

        inline int GetButton() { return m_Button; }

        inline EventTypes  GetEventType() override { return EventTypes::MouseRelease; }
        inline const char* GetEventName() override { return "EventTypes::MouseRelease"; }

        inline std::string GetDetails() override { return fmt::format("MouseReleaseEvent : Button<{0}>", m_Button); }

    private:
        int m_Button;
    };

    class MouseScrollEvent : public MouseEvent
    {
    public:
        MouseScrollEvent(float offset_x, float offset_y) : m_OffsetX(offset_x), m_OffsetY(offset_y) { }

        inline const float GetOffsetX() { return m_OffsetX; }
        inline const float GetOffsetY() { return m_OffsetY; }

        inline EventTypes  GetEventType() override { return EventTypes::MouseScroll; }
        inline const char* GetEventName() override { return "EventTypes::MouseScroll"; }

        inline std::string GetDetails() override
        {
            return fmt::format("MouseScroll : Offsets: X<{0}> Y<{1}>", m_OffsetX, m_OffsetY);
        }

    private:
        float m_OffsetX, m_OffsetY;
    };

    class MouseMoveEvent : public MouseEvent
    {
    public:
        MouseMoveEvent(float x, float y) : m_PosX(x), m_PosY(y) { }

        inline const float GetPosX() { return m_PosX; }
        inline const float GetPosY() { return m_PosY; }

        inline EventTypes  GetEventType() override { return EventTypes::MouseMove; }
        inline const char* GetEventName() override { return "EventTypes::MouseMove"; }

        inline std::string GetDetails() override
        {
            return fmt::format("MouseMove : Positions: X<{0}> Y<{1}>", m_PosX, m_PosY);
        }

    private:
        float m_PosX, m_PosY;
    };

}