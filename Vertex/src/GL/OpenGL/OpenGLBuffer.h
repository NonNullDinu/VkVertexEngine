#pragma once

#include "Renderer/Buffer.h"

namespace Vertex {

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        ~OpenGLVertexBuffer() {}

        void Bind() const override;
        void Unbind() const override;

    private:
        uint32_t m_ID;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t indices, uint32_t size);
        ~OpenGLIndexBuffer() {}

        void Bind() const override;
        void Unbind() const override;

    private:
        uint32_t m_ID;
    };

}