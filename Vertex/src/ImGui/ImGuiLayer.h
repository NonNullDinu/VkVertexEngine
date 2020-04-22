#pragma once

#include "imgui.h"

#include "Core/Layer/Layer.h"

#include "GL/ImGuiRenderer.h"

#include "Core/Application.h"

#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/WindowEvent.h"

namespace Vertex {

    class VX_PUBLIC_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate() override;
        void OnEvent(Event& event) override;

    private:
        float m_Time;

    private:
        // event callbacks
        bool OnWindowResizeEvent(WindowResizeEvent& event);

        bool OnKeyPressEvent(KeyPressEvent& event);
        bool OnKeyReleaseEvent(KeyReleaseEvent& event);
        bool OnKeyCharInputEvent(KeyCharInputEvent& event);

        bool OnMouseClickEvent(MouseClickEvent& event);
        bool OnMouseReleaseEvent(MouseReleaseEvent& event);
        bool OnMouseScrollEvent(MouseScrollEvent& event);
        bool OnMouseMoveEvent(MouseMoveEvent& event);
    };

}