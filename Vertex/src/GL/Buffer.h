#pragma once

#include "Core/Core.h"
#include <GL/Vulkan/VulkanVertexArrayLayout.h>

#include "Shader.h"

namespace Vertex
{

    class Buffer
    {
    public:
        virtual ~Buffer() { }
    };

    // -----------------------------------
    // ---------- Vertex Buffer ----------
    // -----------------------------------

    class VertexBuffer : public Buffer
    {
    public:
        virtual ~VertexBuffer() { }

        virtual void                               Bind() const = 0;
        virtual void                               Unbind() const = 0;
        virtual void                               BeforeRender() = 0;
        virtual void                               AfterRender() = 0;
        template <typename T> static VertexBuffer* Create(std::vector<T> vertices);
        template <typename T> static VertexBuffer* Create(T* vertices, uint32_t count);
        template <typename T>
        static VertexBuffer* Create(std::vector<T> vertices, const VulkanVertexArrayLayoutElement& element);
    };

    // ----------------------------------
    // ---------- Index Buffer ----------
    // ----------------------------------

    class IndexBuffer : public Buffer
    {
    public:
        virtual ~IndexBuffer() { }

        virtual uint32_t GetCount() const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void BeforeRender() = 0;
        virtual void AfterRender() = 0;

        static IndexBuffer* Create(std::vector<uint32_t> indices);
    };

    // -------------------------------------
    // ---------- Uniform Buffer -----------
    // -------------------------------------

    class UniformBuffer : public Buffer
    {
    public:
        virtual ~UniformBuffer() { }

        virtual void BeforeRender() = 0;
        virtual void AfterRender() = 0;
    };
}