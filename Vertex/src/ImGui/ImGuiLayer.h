#pragma once

#include "imgui.h"
#include "Core/Layer/Layer.h"

#include "GL/ImGuiRenderer.hpp" // <- todo

#include "Core/Application.hpp"

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

        bool OnMouseButtonPressEvent(MouseClickEvent& event);
        bool OnMouseButtonReleaseEvent(MouseReleaseEvent& event);
        bool OnMouseScrollEvent(MouseScrollEvent& event);
        bool OnMouseMoveEvent(MouseMoveEvent& event);
    };

}