#include "VulkanBuffer.h"
#include "VulkanContext.h"

namespace Vertex
{
    // --------------------------------
    // --------- Vertex Buffer --------
    // --------------------------------

    // constructor in the header file

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        VkDevice device = VulkanContext::GetContext()->GetDevice();
        vkDestroyBuffer(device, m_InternalVkBuffer, nullptr);
        vkFreeMemory(device, m_BufferMemory, nullptr);
    }

    void VulkanVertexBuffer::BeforeRender()
    {
        VkBuffer     vertexBuffers[] = { m_InternalVkBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(VulkanContext::GetContext()->GetCurrentCommandBuffer(), 0, 1, vertexBuffers, offsets);
    }

    void VulkanVertexBuffer::AfterRender() { }

    // --------------------------
    // ----- Index Buffer -------
    // --------------------------

    VulkanIndexBuffer::VulkanIndexBuffer(std::vector<uint32_t> indices)
    {
        VulkanContext* context = VulkanContext::GetContext();
        VkDeviceSize   buffer_size = sizeof(uint32_t) * indices.size();

        VkBuffer       staging_buffer;
        VkDeviceMemory staging_buffer_memory;
        context->CreateBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer,
            staging_buffer_memory);

        void* data;
        vkMapMemory(context->GetDevice(), staging_buffer_memory, 0, buffer_size, 0, &data);
        memcpy(data, indices.data(), (size_t)buffer_size);
        vkUnmapMemory(context->GetDevice(), staging_buffer_memory);

        context->CreateBuffer(buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_InternalVkBuffer, m_BufferMemory);

        context->CopyBuffer(staging_buffer, m_InternalVkBuffer, buffer_size);

        vkDestroyBuffer(context->GetDevice(), staging_buffer, nullptr);
        vkFreeMemory(context->GetDevice(), staging_buffer_memory, nullptr);

        m_Count = indices.size();
    }

    VulkanIndexBuffer::~VulkanIndexBuffer()
    {
        VkDevice device = VulkanContext::GetContext()->GetDevice();
        vkDestroyBuffer(device, m_InternalVkBuffer, nullptr);
        vkFreeMemory(device, m_BufferMemory, nullptr);
    }

    void VulkanIndexBuffer::Bind() const { }

    void VulkanIndexBuffer::Unbind() const { }

    uint32_t VulkanIndexBuffer::GetCount() const { return m_Count; }
    void     VulkanIndexBuffer::BeforeRender()
    {
        vkCmdBindIndexBuffer(
            VulkanContext::GetContext()->GetCurrentCommandBuffer(), m_InternalVkBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
    void VulkanIndexBuffer::AfterRender() { }
}