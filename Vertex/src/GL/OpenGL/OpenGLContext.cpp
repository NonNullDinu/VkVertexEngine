#include "OpenGLContext.h"

namespace Vertex {

    OpenGLContext::OpenGLContext(GLFWwindow* window)
        : m_WindowHandle(window)
    {
        VX_CORE_ASSERT(m_WindowHandle, "Window handle is null");
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        VX_CORE_ASSERT(status, "Could not initialize glad.");

        CoreLogger::Get()->debug("\nRenderer Info:\n\tRendering API: OpenGL {0}\n\tRenderer: {1}\n\tVendored by: {2}",
                                        glGetString(GL_VERSION),
                                        glGetString(GL_RENDERER),
                                        glGetString(GL_VENDOR));
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);

        if (m_NeedViewportUpdate)
        {
            m_NeedViewportUpdate = false;
            glViewport(0, 0, m_CurrentWidth, m_CurrentHeight);
        }
    }

    void OpenGLContext::NotifyResize(int new_width, int new_height)
    {
        m_CurrentWidth = new_width;
        m_CurrentHeight = new_height;
        m_NeedViewportUpdate = true;
    }

    void OpenGLContext::CleanUpContext()
    {
        
    }
}