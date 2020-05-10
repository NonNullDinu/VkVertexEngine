#include "Buffer.h"

#if defined(VX_RENDER_API_OPENGL)
    #include "OpenGL/OpenGLBuffer.h"
#elif defined(VX_RENDER_API_VULKAN)
    #include "Vulkan/VulkanBuffer.h"
#endif
// ... per rendering API

namespace Vertex
{

    VertexBuffer* VertexBuffer::Create(float* vertices, size_t size, const BufferLayout& layout)
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLVertexBuffer(vertices, size, layout);
#elif defined(VX_RENDER_API_VULKAN)
        return new VulkanVertexBuffer(vertices, size);
#else
        return nullptr; // for now
#endif
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, size_t size)
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLIndexBuffer(indices, size);
#elif defined(VX_RENDER_API_VULKAN)
        return new VulkanIndexBuffer(indices, size);
#else
        return nullptr; // for now
#endif
    }

}