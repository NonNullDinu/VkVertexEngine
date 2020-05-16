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
        template <typename T> VulkanVertexBuffer(std::vector<T> vertices);
        ~VulkanVertexBuffer();
        virtual void Bind() const override {};
        virtual void Unbind() const override {};

        virtual void BeforeRender() override;
        virtual void AfterRender() override;

    private:
        VkBuffer       m_InternalVkBuffer;
        VkDeviceMemory m_BufferMemory;
    };

    template <typename T> VulkanVertexBuffer::VulkanVertexBuffer(std::vector<T> vertices)
    {
        VulkanContext* context = VulkanContext::GetContext();
        VkBuffer       staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        VkDeviceSize   size = sizeof(T) * vertices.size();
        context->CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer,
            staging_buffer_memory);

        void* data;
        vkMapMemory(context->GetDevice(), staging_buffer_memory, 0, size, 0, &data);
        memcpy(data, vertices.data(), size);
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
        VulkanIndexBuffer(uint32_t* indices, uint32_t size);
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
