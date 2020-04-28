#pragma once

namespace Vertex {

    class Buffer
    {
    public:
        virtual ~Buffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };

    class VertexBuffer : public Buffer
    {
    public:
        virtual ~VertexBuffer() {}

        virtual VertexBuffer* Create(float* vertices, uint32_t size);
    };

    class IndexBuffer : public Buffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual IndexBuffer* Create(uint32_t indices, uint32_t size);
    };

}