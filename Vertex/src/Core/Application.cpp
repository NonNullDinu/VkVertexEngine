#include "Application.h"
#include

namespace Vertex {

    Application::Application(IWindow &window) : m_Window(std::make_shared<IWindow>(window))
    {
        m_Window.reset(new GLFWWindowImpl());
        m_Window->SetEventCallback(VX_BIND_FUNC_1(Application::OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event &event)
    {
    }

    void Application::Run()
    {
        while (true){

        }
    }

}