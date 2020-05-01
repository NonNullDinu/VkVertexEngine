#pragma once

#include "Core/Core.h"

#include "Renderer/VertexArray.h"

#include "Renderer/Shader.h"

namespace Vertex {

    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray();

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer> vbo) override;
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer> ibo) override;

    private:
        uint32_t m_ID;

        std::vector<std::shared_ptr<VertexBuffer>> m_VBOs;
        std::shared_ptr<IndexBuffer> m_IBO;
    };

}