#pragma once

#include "GL/Buffer.h"

namespace Vertex {

    // -----------------------------------
    // ---------- Vertex Buffer ----------
    // -----------------------------------

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, size_t size, const BufferLayout& layout);
        ~OpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;

        inline const BufferLayout& GetLayout() const override { return m_Layout; }

    private:
        uint32_t m_ID;
        BufferLayout m_Layout;
    };

    // ----------------------------------
    // ---------- Index Buffer ----------
    // ----------------------------------

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, size_t size);
        ~OpenGLIndexBuffer();

        void Bind() const override;
        void Unbind() const override;

        uint32_t GetCount() const override { return m_Count; }

    private:
        uint32_t m_ID;
        uint32_t m_Count;
    };

}