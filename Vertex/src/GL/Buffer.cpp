#include "Buffer.h"

#if defined(VX_RENDER_API_OPENGL)
    #include "OpenGL/OpenGLBuffer.h"
#endif
// ... per rendering API

namespace Vertex {

    VertexBuffer* VertexBuffer::Create(float* vertices, size_t size, const BufferLayout& layout)
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLVertexBuffer(vertices, size, layout);
#else
        return nullptr; // for now
#endif
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, size_t size)
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLIndexBuffer(indices, size);
#else
        return nullptr; // for now
#endif
    }

}