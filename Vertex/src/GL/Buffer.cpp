#include "Buffer.h"

#include "Vulkan/VulkanBuffer.h"

namespace Vertex
{
    // VertexBuffer::Create implemented in Vulkan/VulkanTemplateImplementations.h

    IndexBuffer* IndexBuffer::Create(std::vector<uint32_t> indices)
    {
        return new VulkanIndexBuffer(indices);
    }

}