#pragma once

#include "Core/Core.h"

#include "Renderer.h"

namespace Vertex {

    class Buffer
    {
    public:
        virtual ~Buffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };

    // -----------------------------------
    // ---------- Vertex Buffer ----------
    // -----------------------------------

    class VertexBuffer : public Buffer
    {
    public:
        virtual ~VertexBuffer() {}

        static VertexBuffer* Create(float* vertices, size_t size);
    };

    // ----------------------------------
    // ---------- Index Buffer ----------
    // ----------------------------------

    class IndexBuffer : public Buffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual uint32_t GetCount() const = 0;

        static IndexBuffer* Create(uint32_t* indices, size_t size);
    };

}

#include "GL/OpenGL/OpenGLBuffer.h"
// ... per rendering API