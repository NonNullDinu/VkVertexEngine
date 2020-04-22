#pragma once

#include "imgui.h"

#include "Core/Layer/Layer.h"

#include "GL/ImGuiRenderer.h" // <- todo

#include "Core/Application.h"

#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"

namespace Vertex {

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate() override;
        void OnEvent(Event& event) override;

    private:
        // event callbacks
        bool OnWindowResizeEvent(WindowResizeEvent& event);

        bool OnKeyPressEvent(KeyPressEvent& event);
        bool OnKeyReleaseEvent(KeyReleaseEvent& event);
        bool OnKeyCharInputEvent(KeyCharInput& event);

        bool OnMouseClickEvent(MouseClickEvent& event);
        bool OnMouseReleaseEvent(MouseReleaseEvent& event);
        bool OnMouseScrollEvent(MouseScrollEvent& event);
        bool OnMouseMoveEvent(MouseMoveEvent& event);
    };

}