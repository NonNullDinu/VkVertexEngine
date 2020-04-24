#include "Application.h"

#include <glm/glm.hpp>

namespace Vertex {

    Application* Application::s_AppInstance = nullptr;

    Application::Application()
        : m_Running(true)
    {
        VX_CORE_ASSERT((!s_AppInstance), "Application cannot be instantiated twice!")

        m_Window.reset(new WindowImpl());
        m_Window->SetEventCallback(VX_BIND_FUNC_1(Application::OnEvent));

        s_AppInstance = this;

        ImGuiLayer* m_ImGuiLayer = new ImGuiLayer;
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& event)
    {
        EventHandler handler(event);
        handler.Dispatch<EventTypes::WindowClose, WindowCloseEvent>(VX_BIND_FUNC_1(Application::OnWindowCloseEvent));

        for (std::vector<Layer*>::iterator it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(event);
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.001f, 0.001f, 0.02f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            if (Input::IsKeyPressed(VX_KEY_SPACE))
                Logger::GetCoreLogger()->debug("Space bar pressed");

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();

            for (Layer* layer : m_LayerStack)
                layer->OnImguiRender();
            
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }

        return;
    }

    // Application specific event callbacks
    bool Application::OnWindowCloseEvent(Event& event)
    {
        m_Running = false;
        return true;
    }

}