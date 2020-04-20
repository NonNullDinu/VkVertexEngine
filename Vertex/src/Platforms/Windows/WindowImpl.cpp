#include "WindowImpl.h"
#include <Core/Logger.h>
#include <glad/glad.h>
#include <Core/Event/KeyEvent.h>
#include <Core/Event/MouseEvent.h>

namespace Vertex {

    static bool s_GLFW_Initialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        Logger::GetCoreLogger()->error("GLFW error {0} : {1}", error, description);
    }
    namespace GLFWInputCallbacks {
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);
            if (properties == nullptr)
            {
                Logger::GetCoreLogger()->error("Null window properties");
                return;
            }
            if (action == GLFW_PRESS)
            {
                KeyPressEvent e(key, 1);
                properties->event_callback(e);
            }
            else if (action == GLFW_RELEASE)
            {
                KeyReleaseEvent e(key);
                properties->event_callback(e);
            }
            else
            {
                Logger::GetCoreLogger()->info("Got something weird from glfw(key callback): {0}", action);
                return;
            }
        }
        static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);
            if (properties == nullptr)
            {
                Logger::GetCoreLogger()->error("Null window properties");
                return;
            }
            MouseMoveEvent e((int)xpos, (int)ypos);
            properties->event_callback(e);
        }
        void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);
            if (properties == nullptr)
            {
                Logger::GetCoreLogger()->error("Null window properties");
                return;
            }
            if (action == GLFW_PRESS)
            {
                MouseClickEvent e(button);
                properties->event_callback(e);
            }
            else if (action == GLFW_RELEASE)
            {
                MouseReleaseEvent e(button);
                properties->event_callback(e);
            }
            else
            {
                Logger::GetCoreLogger()->info("Got something weird from glfw(mouse callback): {0}", action);
                return;
            }
        }

        void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);
            if (properties == nullptr)
            {
                Logger::GetCoreLogger()->error("Null window properties");
                return;
            }
            MouseScrollEvent e(xoffset, yoffset);
            properties->event_callback(e);
        }
    }

    WindowImpl::WindowImpl(WindowProperties properties)
        : m_Data(properties)
    {
        Logger::GetCoreLogger()->info("Creating window {}", m_Data.title);

        if (!s_GLFW_Initialized)
        {
            int success = glfwInit();

            if (!success)
                Logger::GetCoreLogger()->error("Could not initialize GLFW");

            s_GLFW_Initialized = true;
        }
        glfwSetErrorCallback(GLFWErrorCallback);

        m_Window = glfwCreateWindow((int)m_Data.width, (int)m_Data.height, m_Data.title, nullptr, nullptr);

        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            Logger::GetCoreLogger()->error("Could not initialize glad.");

        glfwSetWindowUserPointer(m_Window, &m_Data);

        // set up glfw input callbacks
        glfwSetKeyCallback(m_Window, GLFWInputCallbacks::key_callback); // keys
        glfwSetCursorPosCallback(m_Window, GLFWInputCallbacks::cursor_position_callback); // mouse movement
        glfwSetMouseButtonCallback(m_Window, GLFWInputCallbacks::mouse_button_callback); // mouse buttons
        glfwSetScrollCallback(m_Window, GLFWInputCallbacks::scroll_callback); // mouse scroll
    }

    WindowImpl::~WindowImpl()
    {
        ShutDown();
    }

    void WindowImpl::ShutDown()
    {
        glfwDestroyWindow(m_Window); // we may want to shutdown before the window's scope is over
    }

    void WindowImpl::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

}