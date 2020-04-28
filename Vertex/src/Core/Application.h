#pragma once

#include "Core.h"

class ImGuiLayer; // <- forward declare it to be accessible from application

#include "Event/Event.h"

#include "Event/WindowEvent.h"

#include "Layer/LayerStack.h"

#include "ImGui/ImGuiLayer.h"

#include "GL/OpenGL/Shader.h"

#if defined(_WIN32)
	#include "Platforms/Windows/WindowImpl.h"
#elif defined(__linux__)
	#include "Platforms/Linux/WindowImpl.h"
#else
	#error Unsupported platform.
#endif

namespace Vertex {

    class VX_PUBLIC_API Application
	{
    public:
        Application();
        ~Application();

        inline IWindow& GetWindow() { return *m_Window; }
        
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

        std::shared_ptr<IWindow> m_Window;

        static Application* s_AppInstance;
        // ^ application is a singleton class

        // ------------- Temporary --------------

        unsigned int m_VertexArr, m_VertexBuf, m_IndexBuf;
        std::unique_ptr<Shader> m_Shader;

        // --------------------------------------

    private:
        // Application specific event callbacks
        bool OnWindowCloseEvent(Event& event);
        bool OnWindowResizeEvent(WindowResizeEvent& event);
    };

}