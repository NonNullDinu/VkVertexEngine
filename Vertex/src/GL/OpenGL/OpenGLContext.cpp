#include "OpenGLContext.h"

namespace Vertex {

    OpenGLContext::OpenGLContext(GLFWwindow* window)
        : m_WindowHandle(window)
    {
        VX_CORE_ASSERT(m_WindowHandle, "Window handle is null");
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        VX_CORE_ASSERT(status, "Could not initialize glad.");
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}