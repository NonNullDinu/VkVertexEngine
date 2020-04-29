#pragma once

#include "Renderer/Buffer.h"

namespace Vertex {

    // -----------------------------------
    // ---------- Vertex Buffer ----------
    // -----------------------------------

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, size_t size);
        ~OpenGLVertexBuffer();

        void Bind() const override;
        void Unbind() const override;

    private:
        uint32_t m_ID;
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