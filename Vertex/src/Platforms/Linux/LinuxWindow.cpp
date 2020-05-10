#include "LinuxWindow.h"

#include "Core/Event/WindowEvent.h"
#include "Core/Event/KeyEvent.h"
#include "Core/Event/MouseEvent.h"

namespace Vertex
{

    static bool s_GLFW_Initialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        CoreLogger::Get()->error("GLFW error {0} : {1}", error, description);
    }

    namespace GLFWInputCallbacks
    {

        static void WindowSizeCallback(GLFWwindow* window, int width, int height)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);

            properties->width = width;
            properties->height = height;

            WindowResizeEvent e(width, height);
            properties->event_callback(e);
        }

        static void WindowCloseCallback(GLFWwindow* window)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);
            WindowCloseEvent e;
            properties->event_callback(e);
        }

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);

            if (properties == nullptr)
            {
                CoreLogger::Get()->error("Null window properties");
                return;
            }

            switch (action)
            {
            case GLFW_PRESS:
            {
                KeyPressEvent e(key, 0);
                properties->event_callback(e);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressEvent e(key, 1); // set to 1 for now
                properties->event_callback(e);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleaseEvent e(key);
                properties->event_callback(e);
                break;
            }
            }
        }

        static void KeyCharInputCallback(GLFWwindow* window, unsigned int codepoint)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);
            KeyCharInputEvent e(codepoint);
            properties->event_callback(e);
        }

        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);

            if (properties == nullptr)
            {
                CoreLogger::Get()->error("Null window properties");
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
        }

        static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);

            if (properties == nullptr)
            {
                CoreLogger::Get()->error("Null window properties");
                return;
            }

            MouseScrollEvent e(xoffset, yoffset);
            properties->event_callback(e);
        }

        static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);

            if (properties == nullptr)
            {
                CoreLogger::Get()->error("Null window properties");
                return;
            }

            MouseMoveEvent e(xpos, ypos);
            properties->event_callback(e);
        }
        static void WindowFocusCallback(GLFWwindow* window, int focused)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);

            if (properties == nullptr)
            {
                CoreLogger::Get()->error("Null window properties");
                return;
            }

            if (focused == GLFW_TRUE)
            {
                WindowGainedFocusEvent e;
                properties->event_callback(e);
            }
            else if (focused == GLFW_FALSE)
            {
                WindowLostFocusEvent e;
                properties->event_callback(e);
            }
        }
        void CursorEnterLeaveCallback(GLFWwindow* window, int entered)
        {
            WindowProperties* properties = (WindowProperties*)glfwGetWindowUserPointer(window);

            if (properties == nullptr)
            {
                CoreLogger::Get()->error("Null window properties");
                return;
            }

            if (entered == GLFW_TRUE)
            {
                WindowCursorEnteredEvent e;
                properties->event_callback(e);
            }
            else
            {
                WindowCursorLeftEvent e;
                properties->event_callback(e);
            }
        }

    }

    LinuxWindow::LinuxWindow(WindowProperties properties)
        : m_Data(properties)
    {
        CoreLogger::Get()->info("Creating window {0}...", m_Data.title);

        if (!s_GLFW_Initialized)
        {
            int success = glfwInit();

            if (!success)
                CoreLogger::Get()->error("Could not initialize GLFW");

            s_GLFW_Initialized = true;
        }

        glfwSetErrorCallback(GLFWErrorCallback);
#if defined(VX_RENDER_API_VULKAN)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

        m_Window = glfwCreateWindow((int)m_Data.width, (int)m_Data.height, m_Data.title, nullptr, nullptr);
        m_Context.reset(GraphicsContext::Create(m_Window));

        glfwSetWindowUserPointer(m_Window, &m_Data);

        // set up glfw input callbacks
        glfwSetWindowSizeCallback(m_Window, GLFWInputCallbacks::WindowSizeCallback);
        glfwSetWindowCloseCallback(m_Window, GLFWInputCallbacks::WindowCloseCallback);
        glfwSetWindowFocusCallback(m_Window, GLFWInputCallbacks::WindowFocusCallback);
        glfwSetCursorEnterCallback(m_Window, GLFWInputCallbacks::CursorEnterLeaveCallback);

        glfwSetKeyCallback(m_Window, GLFWInputCallbacks::KeyCallback);
        glfwSetCharCallback(m_Window, GLFWInputCallbacks::KeyCharInputCallback);

        glfwSetMouseButtonCallback(m_Window, GLFWInputCallbacks::MouseButtonCallback);
        glfwSetScrollCallback(m_Window, GLFWInputCallbacks::MouseScrollCallback);
        glfwSetCursorPosCallback(m_Window, GLFWInputCallbacks::CursorPositionCallback);
    }

    LinuxWindow::~LinuxWindow()
    {
        ShutDown();
    }

    void LinuxWindow::ShutDown()
    {
        glfwDestroyWindow(m_Window); // we may want to shutdown before the window's scope is over
    }

    void LinuxWindow::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void LinuxWindow::OnEvent(Event& event)
    {
        EventHandler handler(event);
        handler.Dispatch<EventTypes::WindowResize, WindowResizeEvent>(VX_BIND_FUNC_1(LinuxWindow::OnWindowResizeEvent));
    }

    // event callbacks
    bool LinuxWindow::OnWindowResizeEvent(WindowResizeEvent& event)
    {
        m_Context->NotifyResize(event.GetWidth(), event.GetHeight());
        return false; // <- for now
    }

}