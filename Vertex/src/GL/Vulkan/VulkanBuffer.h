#pragma once

#include "GL/Buffer.h"
#include "VulkanContext.h"

namespace Vertex
{
    // --------------------------------
    // --------- Vertex Buffer --------
    // --------------------------------

    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        template <typename T> VulkanVertexBuffer(const std::vector<T>& vertices_data);
        template <typename T> VulkanVertexBuffer(T* vertices_data, uint32_t vertices_count);
        template <typename T>
        VulkanVertexBuffer(const std::vector<T>& vertices_data, const VulkanVertexArrayLayoutElement& element);
        ~VulkanVertexBuffer();
        virtual void Bind() const override {};
        virtual void Unbind() const override {};

        virtual void BeforeRender() override;
        virtual void AfterRender() override;

    private:
        VkBuffer       m_InternalVkBuffer;
        VkDeviceMemory m_BufferMemory;
    };

    template <typename T>
    VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<T>& vertices_data)
        : VulkanVertexBuffer(vertices_data.data(), vertices_data.size())
    {
    }
    template <typename T> VulkanVertexBuffer::VulkanVertexBuffer(T* vertices_data, uint32_t vertices_count)
    {
        VulkanContext* context = VulkanContext::GetContext();
        VkBuffer       staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        VkDeviceSize   size = sizeof(T) * vertices_count;
        context->CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer,
            staging_buffer_memory);

        VxPtr data;
        vkMapMemory(context->GetDevice(), staging_buffer_memory, 0, size, 0, &data);
        memcpy(data, vertices_data, size);
        vkUnmapMemory(context->GetDevice(), staging_buffer_memory);

        context->CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_InternalVkBuffer, m_BufferMemory);
        context->CopyBuffer(staging_buffer, m_InternalVkBuffer, size);

        vkDestroyBuffer(context->GetDevice(), staging_buffer, nullptr);
        vkFreeMemory(context->GetDevice(), staging_buffer_memory, nullptr);
    }

    template <typename T>
    VulkanVertexBuffer::VulkanVertexBuffer(
        const std::vector<T>& vertices_data, const VulkanVertexArrayLayoutElement& element)
    {
        VulkanContext* context = VulkanContext::GetContext();
        VkBuffer       staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        VkDeviceSize   size = element.internal_size * vertices_data.size();
        context->CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer,
            staging_buffer_memory);

        VxPtr data;
        vkMapMemory(context->GetDevice(), staging_buffer_memory, 0, size, 0, &data);
        VxArithPtr a_data = (VxArithPtr)data;
        VxArithPtr v_data = (VxArithPtr)vertices_data.data();
        for (VxSize i = 0; i < vertices_data.size(); i++)
            memcpy(a_data + i * element.internal_size, v_data + i * sizeof(T) + element.vertex_offset,
                element.internal_size);
        vkUnmapMemory(context->GetDevice(), staging_buffer_memory);

        context->CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_InternalVkBuffer, m_BufferMemory);
        context->CopyBuffer(staging_buffer, m_InternalVkBuffer, size);

        vkDestroyBuffer(context->GetDevice(), staging_buffer, nullptr);
        vkFreeMemory(context->GetDevice(), staging_buffer_memory, nullptr);
    }

    // -----------------------------------
    // --------- Index Buffer ------------
    // -----------------------------------
    class VulkanIndexBuffer : public IndexBuffer
    {
    public:
        VulkanIndexBuffer(std::vector<uint32_t> indices);
        ~VulkanIndexBuffer();
        virtual void     Bind() const override;
        virtual void     Unbind() const override;
        virtual uint32_t GetCount() const override;

        virtual void BeforeRender() override;
        virtual void AfterRender() override;

    private:
        VkBuffer       m_InternalVkBuffer;
        VkDeviceMemory m_BufferMemory;
        uint32_t       m_Count;
    };
}
