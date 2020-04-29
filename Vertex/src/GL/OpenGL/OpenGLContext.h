#pragma once

#include "Core/Core.h"

#include "Renderer/GraphicsContext.h"

namespace Vertex {

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* window);

        void SwapBuffers() override;

        void NotifyResize(int new_width, int new_height) override;

    private:
        GLFWwindow* m_WindowHandle;

        int m_CurrentWidth, m_CurrentHeight;
        bool m_NeedViewportUpdate;
    };

}