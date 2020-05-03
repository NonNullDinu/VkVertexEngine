#include "GraphicsContext.h"

namespace Vertex {

    GraphicsContext* GraphicsContext::Create(GLFWwindow* window_handle)
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLContext(window_handle);
#elif defined(VX_RENDER_API_DIRECTX12)
        return new DirectX12Context(window_handle);
#else
        return nullptr;
#endif
    }

}