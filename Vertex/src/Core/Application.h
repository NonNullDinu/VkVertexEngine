#pragma once

#include "Core.h"

class ImGuiLayer; // <- forward declare it to be accessible from application

#include "Event/Event.h"
#include "Event/WindowEvent.h"

#include "Layer/LayerStack.h"

#include "ImGui/ImGuiLayer.h"

#include "Window.h"

#include "GL/Buffer.h"
#include "GL/VertexArray.h"
#include "GL/Shader.h"

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

        std::shared_ptr<Shader> m_Shader;

        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;

        std::shared_ptr<VertexArray> m_VertexArray2;
        std::shared_ptr<VertexBuffer> m_VertexBuffer2;
        std::shared_ptr<IndexBuffer> m_IndexBuffer2;

        // --------------------------------------

        void Render();

    private:
        // Application specific event callbacks
        bool OnWindowCloseEvent(Event& event);
    };

}
