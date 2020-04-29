#pragma once

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
    };

    // ----------------------------------
    // ---------- Index Buffer ----------
    // ----------------------------------

    class IndexBuffer : public Buffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual uint32_t GetCount() const = 0;
    };

}