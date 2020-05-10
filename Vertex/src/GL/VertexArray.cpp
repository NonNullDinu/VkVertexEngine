#include "VertexArray.h"

#if defined(VX_RENDER_API_OPENGL)
    #include "OpenGL/OpenGLVertexArray.h"
#elif defined(VX_RENDER_API_VULKAN)
    #include "Vulkan/VulkanVertexArray.h"
#endif
// ... per rendering api

namespace Vertex
{

    VertexArray* VertexArray::Create()
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLVertexArray();
#elif defined(VX_RENDER_API_VULKAN)
        return new VulkanVertexArray();
#else
        return nullptr;
#endif
    }

}