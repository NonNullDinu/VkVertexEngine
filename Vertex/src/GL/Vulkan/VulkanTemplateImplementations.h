#pragma once

#include "VulkanBuffer.h"
#include "VulkanContext.h"

namespace Vertex
{
    template <typename T> VertexBuffer* VertexBuffer::Create(std::vector<T> vertices)
    {
        return new VulkanVertexBuffer(vertices);
    }
}