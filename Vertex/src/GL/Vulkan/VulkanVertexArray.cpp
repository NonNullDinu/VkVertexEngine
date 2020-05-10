#include "VulkanVertexArray.h"
namespace Vertex
{
    VulkanVertexArray::VulkanVertexArray()
    {

    }
    void VulkanVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vbo)
    {

    }
    void VulkanVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> ibo)
    {
        m_IndexBuffer = ibo;
    }
    const std::shared_ptr<IndexBuffer>& VulkanVertexArray::GetIndexBuffer() const
    {
        return m_IndexBuffer;
    }
}
