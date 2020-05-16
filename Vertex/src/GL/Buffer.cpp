#include "Buffer.h"

#include "Vulkan/VulkanBuffer.h"

namespace Vertex
{
    // VertexBuffer::Create implemented in Vulkan/VulkanTemplateImplementations.h

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, size_t size) { return new VulkanIndexBuffer(indices, size); }

}