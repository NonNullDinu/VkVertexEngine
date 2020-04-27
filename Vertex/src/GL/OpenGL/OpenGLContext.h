#pragma once

#include "Core/Core.h"

#include "Core/Window.h"

#include "Renderer/GraphicsContext.h"

namespace Vertex {

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* window);

        void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };

}