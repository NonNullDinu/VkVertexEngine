#pragma once

#include "Core/Core.h"

#include "Shader.h"

namespace Vertex {

    // -----------------------------------
    // ---------- Buffer Layout ----------
    // -----------------------------------

    struct BufferElement
    {
        ShaderDataType type;
        uint32_t offset;
        uint32_t size;

        BufferElement(ShaderDataType _type)
            : type(_type), size(GetSizeOfShaderDataType(_type)), offset(0)
        {
        }
    };
    

    class BufferLayout
    {
    public:
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Elements(elements)
        {
        }

        inline const std::vector<BufferElement>& GetElements() { return m_Elements; }

    private:
        void CalculateOffsetsAndStride()
        {
            uint32_t offset = 0;
            for (auto& elem : m_Elements)
            {
                elem.offset = offset;
                offset += elem.size;
                m_Stride += elem.size;
            }
        }

    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    class Buffer
    {
    public:
        virtual ~Buffer() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };

    // -----------------------------------
    // ---------- Vertex Buffer ----------
    // -----------------------------------

    class VertexBuffer : public Buffer
    {
    public:
        virtual ~VertexBuffer() {}

        static VertexBuffer* Create(float* vertices, size_t size);
    };

    // ----------------------------------
    // ---------- Index Buffer ----------
    // ----------------------------------

    class IndexBuffer : public Buffer
    {
    public:
        virtual ~IndexBuffer() {}

        virtual uint32_t GetCount() const = 0;

        static IndexBuffer* Create(uint32_t* indices, size_t size);
    };

}

#include "GL/OpenGL/OpenGLBuffer.h"
// ... per rendering API