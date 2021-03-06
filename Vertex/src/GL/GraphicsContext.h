#pragma once

#include "Core/Core.h"

namespace Vertex
{

    class GraphicsContext
    {
        // the this pointer from GraphicsContext::Render
        typedef std::function<void(GraphicsContext*)> PrepareRenderFunc;
        typedef std::function<void(GraphicsContext*)> RenderFunc;

    public:
        virtual ~GraphicsContext() = default;

        virtual void Render() = 0;

        virtual void SwapBuffers() = 0;

        virtual void NotifyResize(int new_width, int new_height) = 0;

        virtual void CleanUpContext() = 0;

        inline void SetPrepareRenderCallback(PrepareRenderFunc callback) { m_PrepareRenderCallback = callback; }
        inline void SetRenderCallback(RenderFunc callback) { m_RenderCallback = callback; }

        static GraphicsContext* Create(GLFWwindow* window_handle);

    protected:
        RenderFunc        m_RenderCallback;
        PrepareRenderFunc m_PrepareRenderCallback;
    };

}