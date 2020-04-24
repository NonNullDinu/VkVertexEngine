#pragma once

#include <imgui.h>

#include <examples/imgui_impl_glfw.h>    // imgui
#include <examples/imgui_impl_opengl3.h> // imgui

#include "Core/Core.h"

#include "Core/Layer/Layer.h"

#include "Core/Application.h"

#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"
#include "Core/Event/WindowEvent.h"

namespace Vertex {

    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach() override;
        void OnDetach() override;

        void OnImguiRender() override;

        void Begin();
        void End();

    private:
        float m_Time;
    };

}