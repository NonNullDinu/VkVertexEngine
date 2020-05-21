#pragma once

#include "VulkanBuffer.h"
#include "VulkanContext.h"
#include "VulkanVertexArray.h"
#include "VulkanVertexArrayLayout.h"

namespace Vertex
{
    template <typename T> VertexBuffer* VertexBuffer::Create(std::vector<T> vertices)
    {
        return new VulkanVertexBuffer(vertices);
    }
    template <typename T> VertexBuffer* VertexBuffer::Create(T* vertices, uint32_t count)
    {
        return new VulkanVertexBuffer(vertices, count);
    }
    template <typename T>
    VertexBuffer* VertexBuffer::Create(std::vector<T> vertices, const VulkanVertexArrayLayoutElement& element)
    {
        return new VulkanVertexBuffer(vertices, element);
    }
    template <VertexDataConcept T>
    VertexArray* VertexArray::Create(std::vector<T> vertices, std::vector<uint32_t> indices)
    {
        VulkanVertexArrayLayout layout = T::GetVertexArrayLayout();

        VertexArray* array = VertexArray::Create();

        for (auto layout_elem : layout)
        {
            std::shared_ptr<VertexBuffer> ptr;
            ptr.reset(VertexBuffer::Create(vertices, layout_elem));
            array->AddVertexBuffer(ptr);
        }

        std::shared_ptr<IndexBuffer> ptr;
        ptr.reset(IndexBuffer::Create(indices));
        array->SetIndexBuffer(ptr);

        return array;
    }
}