#include "Application.h"

namespace Vertex {

    Application::Application()
    {
        m_Window.reset(new WindowImpl());
        m_Window->SetEventCallback(VX_BIND_FUNC_1(Application::OnEvent));
        this->m_Running = true;
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& event)
    {
        Logger::GetCoreLogger()->debug(event.GetDetails());

        if (event.GetEventType() == EventTypes::WindowClose)
        {
            EventHandler handler(event);
            handler.Dispatch<EventTypes::WindowClose>(VX_BIND_FUNC_1(Application::OnWindowCloseEvent));
        }

        for (std::vector<Layer*>::iterator it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_Window->OnUpdate();
        }
    }

    // Application specific event callbacks
    bool Application::OnWindowCloseEvent(Event& event)
    {
        m_Running = false;
        return true;
    }

}