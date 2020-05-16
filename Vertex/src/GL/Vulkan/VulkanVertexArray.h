#pragma once

// since vulkan doesn't have vaos we have to implement the functionality ourselves

#include "GL/VertexArray.h"

namespace Vertex
{
    class VulkanVertexArray : public VertexArray
    {
    public:
        VulkanVertexArray();
        virtual ~VulkanVertexArray() = default;

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer> vbo) override;
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer> ibo) override;

        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;

        void BeforeRender() override;
        void AfterRender() override;

    private:
        std::shared_ptr<IndexBuffer>               m_IndexBuffer;
        std::vector<std::shared_ptr<VertexBuffer>> m_VBOs;
    };
}
