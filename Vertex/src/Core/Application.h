#pragma once

#include "Core.h"

#include "Event/Event.h"
#include "Event/WindowEvent.h"

#include "Layer/LayerStack.h"

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

        inline void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); layer->OnAttach(); }
        inline void PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }
        
        inline void PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); overlay->OnAttach(); }
        inline void PopOverlay(Layer* overlay) { m_LayerStack.PopOverlay(overlay); }

        void OnEvent(Event& event);

        void Run();

        inline IWindow& GetWindow() { return *m_Window; }

    private:
        bool m_Running;
        // we want this^ variable since the application might close for multiple reasons
        LayerStack m_LayerStack;

        std::shared_ptr<IWindow> m_Window;

    private:
        // Application specific event callbacks
        bool OnWindowCloseEvent(Event& event);
    };

}