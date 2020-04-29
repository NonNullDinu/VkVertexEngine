#pragma once

#include "Core.h"

class ImGuiLayer; // <- forward declare it to be accessible from application

#include "Event/Event.h"
#include "Event/WindowEvent.h"

#include "Layer/LayerStack.h"

#include "ImGui/ImGuiLayer.h"

#include "Platforms/Linux/LinuxWindow.h"
#include "Platforms/Windows/WindowsWindow.h"

#include "GL/OpenGL/OpenGLShader.h"
#include "GL/OpenGL/OpenGLBuffer.h"

// ------------- TODOS -------------
/*
* Abstract Shader class
* Abstract Window class properly
*/
// ---------------------------------

namespace Vertex {

    class VX_PUBLIC_API Application
	{
    public:
        Application();
        ~Application();

        inline Window& GetWindow() { return *m_Window; }
        
        static inline Application& Get() { return *s_AppInstance; }

        inline void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); layer->OnAttach(); }
        inline void PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }
        
        inline void PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); overlay->OnAttach(); }
        inline void PopOverlay(Layer* overlay) { m_LayerStack.PopOverlay(overlay); }

        void OnEvent(Event& event);

        void Run();

    private:
        bool m_Running;
        // we want this^ variable since the application might close for multiple reasons
        LayerStack m_LayerStack;

        ImGuiLayer* m_ImGuiLayer;

        std::shared_ptr<Window> m_Window;

        static Application* s_AppInstance;
        // ^ application is a singleton class

        // ------------- Temporary --------------

        unsigned int m_VertexArr;

        std::unique_ptr<Shader> m_Shader;

        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;

        // --------------------------------------

    private:
        // Application specific event callbacks
        bool OnWindowCloseEvent(Event& event);
        bool OnWindowResizeEvent(WindowResizeEvent& event);
    };

}