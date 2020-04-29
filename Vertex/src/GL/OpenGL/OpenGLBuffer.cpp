#include "OpenGLBuffer.h"

namespace Vertex {

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) 
    {
        glCreateBuffers(1, &m_ID);
        glBufferData(GL_ARRAY_BUFFER, size, &vertices, GL_STATIC_DRAW);
    }

    void OpenGLVertexBuffer::Bind() const
    {

    }

    void OpenGLVertexBuffer::Unbind() const
    {

    }

}