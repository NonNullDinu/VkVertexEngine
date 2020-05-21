#pragma once

#include "Buffer.h"

namespace Vertex
{
    template <typename T> concept VertexDataConcept = requires { T::GetVertexArrayLayout(); };

    class VertexArray
    {
    public:
        virtual ~VertexArray() { }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void BeforeRender() = 0;
        virtual void AfterRender() = 0;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer> vbo) = 0;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> ibo) = 0;

        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

        static VertexArray* Create();
        template <VertexDataConcept T>
        static VertexArray* Create(std::vector<T> vertices, std::vector<uint32_t> indices);
    };

}