#include "VulkanVertexArray.h"
namespace Vertex
{
    VulkanVertexArray::VulkanVertexArray() { }
    void VulkanVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vbo) { m_VBOs.push_back(vbo); }
    void VulkanVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> ibo) { m_IndexBuffer = ibo; }
    const std::shared_ptr<IndexBuffer>& VulkanVertexArray::GetIndexBuffer() const { return m_IndexBuffer; }
    void                                VulkanVertexArray::Bind() const
    {
        m_VBOs[0]->BeforeRender();
        m_IndexBuffer->BeforeRender();
    }
    void VulkanVertexArray::Unbind() const
    {
        m_IndexBuffer->AfterRender();
        m_VBOs[0]->AfterRender();
    }
}
