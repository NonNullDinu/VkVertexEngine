#include "GraphicsContext.h"

#if defined(VX_RENDER_API_OPENGL)
    #include "OpenGL/OpenGLContext.h"
#elif defined(VX_RENDER_API_VULKAN)
    #include "Vulkan/VulkanContext.h"
#elif defined(VX_RENDER_API_DIRECTX12)
    #include "DirectX12/DirectX12Context.h"
#endif
// ... per rendering API

namespace Vertex
{

    GraphicsContext* GraphicsContext::Create(GLFWwindow* window_handle)
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLContext(window_handle);
#elif defined(VX_RENDER_API_VULKAN)
        return new VulkanContext(window_handle);
#elif defined(VX_RENDER_API_DIRECTX12)
        return new DirectX12Context(window_handle);
#else
        return nullptr;
#endif
    }

}