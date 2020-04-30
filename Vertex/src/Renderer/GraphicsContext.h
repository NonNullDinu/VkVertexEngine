#pragma once

#include "Core/Core.h"

namespace Vertex {

    class GraphicsContext
    {
        typedef std::function<void(
            GraphicsContext* // the this pointer from GraphicsContext::Render
#if defined(VX_RENDER_API_VULKAN) // if we use vk we need the command buffer too
            , VkCommandBuffer
#endif
        )> RenderFunc;
    protected:
        RenderFunc m_RenderCallback;
    public:
        virtual void Render() = 0;
        virtual void SwapBuffers() = 0;

        virtual void NotifyResize(int new_width, int new_height) = 0;

        virtual void CleanUpContext() = 0;
    };

}