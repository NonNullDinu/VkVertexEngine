#pragma once

#include "Core/Core.h"

#include "Shader.h"

namespace Vertex
{

    class Buffer
    {
    public:
        virtual ~Buffer() { }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };

    // -----------------------------------
    // ---------- Vertex Buffer ----------
    // -----------------------------------

    class VertexBuffer : public Buffer
    {
    public:
        virtual ~VertexBuffer() { }

        virtual void                               BeforeRender() = 0;
        virtual void                               AfterRender() = 0;
        template <typename T> static VertexBuffer* Create(std::vector<T> vertices);
    };

    // ----------------------------------
    // ---------- Index Buffer ----------
    // ----------------------------------

    class IndexBuffer : public Buffer
    {
    public:
        virtual ~IndexBuffer() { }

        virtual uint32_t GetCount() const = 0;

        virtual void BeforeRender() = 0;
        virtual void AfterRender() = 0;

        static IndexBuffer* Create(uint32_t* indices, size_t size);
    };

}