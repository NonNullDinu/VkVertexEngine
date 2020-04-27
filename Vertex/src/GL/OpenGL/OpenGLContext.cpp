#include "OpenGLContext.h"

namespace Vertex {

    OpenGLContext::OpenGLContext(GLFWwindow* window)
        : m_WindowHandle(window)
    {
        VX_CORE_ASSERT(m_WindowHandle, "Window handle is null");
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        VX_CORE_ASSERT(status, "Could not initialize glad.");

        Logger::GetCoreLogger()->info("OpenGL {0} {1} from {2}",
                                        glGetString(GL_VERSION),
                                        glGetString(GL_RENDERER),
                                        glGetString(GL_VENDOR));
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

}