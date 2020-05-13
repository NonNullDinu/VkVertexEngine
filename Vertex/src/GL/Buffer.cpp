#include "Buffer.h"

#include "Vulkan/VulkanBuffer.h"

namespace Vertex
{

    VertexBuffer* VertexBuffer::Create(float* vertices, size_t size, const BufferLayout& layout)
    {
        return new VulkanVertexBuffer(vertices, size);
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, size_t size) { return new VulkanIndexBuffer(indices, size); }

}