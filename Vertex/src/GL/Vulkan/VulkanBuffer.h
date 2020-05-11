#pragma once

#include "GL/Buffer.h"

namespace Vertex
{
    // --------------------------------
    // --------- Vertex Buffer --------
    // --------------------------------

    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        VulkanVertexBuffer(float* vertices, size_t size);
        ~VulkanVertexBuffer();
        virtual void Bind() const override {};
        virtual void Unbind() const override {};

        virtual void                BeforeRender() override;
        virtual void                AfterRender() override;
        virtual const BufferLayout& GetLayout() const override;

    private:
        VkBuffer       m_InternalVkBuffer;
        VkDeviceMemory m_BufferMemory;
    };

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
