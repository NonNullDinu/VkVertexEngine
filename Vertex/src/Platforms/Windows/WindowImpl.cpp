#include "WindowImpl.h"

namespace Vertex {

    static bool s_GLFW_Initialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        Logger::GetCoreLogger()->error("GLFW error {0} : {1}", error, description);
    }

    Window::Window(WindowProperties properties)
        : m_Data(properties)
    {
        Logger::GetCoreLogger()->info("Creating window {}", m_Data.title);

        if (!s_GLFW_Initialized)
        {
            int success = glfwInit();

            if (!success)
                Logger::GetCoreLogger()->error("Could not inisialize GLFW");

            s_GLFW_Initialized = true;
        }

        m_Window = glfwCreateWindow((int)m_Data.width, (int)m_Data.height, m_Data.title, nullptr, nullptr);

        glfwMakeContextCurrent(m_Window);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            Logger::GetCoreLogger()->error("Could not initialize glad.");

        glfwSetWindowUserPointer(m_Window, &m_Data);
    }

    Window::~Window()
    {
        ShutDown();
    }

    void Window::ShutDown()
    {
        glfwDestroyWindow(m_Window); // we may want to shutdown before the window's scope is over
    }

    void Window::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

}