#include "Buffer.h"

#include "GL/OpenGL/OpenGLBuffer.h"

namespace Vertex {

    VertexBuffer* VertexBuffer::Create(float* vertices, size_t size)
    {
        return new OpenGLVertexBuffer(vertices, size);
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, size_t size)
    {
        return new OpenGLIndexBuffer(indices, size);
    }

}