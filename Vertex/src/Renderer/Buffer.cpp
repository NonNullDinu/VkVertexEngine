#include "Buffer.h"

namespace Vertex {

    VertexBuffer* VertexBuffer::Create(float* vertices, size_t size)
    {
        switch(Renderer::GetRenderAPI())
        {
            case RenderAPI::OpenGL : return new OpenGLVertexBuffer(vertices, size);
            case RenderAPI::Vulkan : return nullptr;
        }

        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, size_t size)
    {
        switch(Renderer::GetRenderAPI())
        {
            case RenderAPI::OpenGL : return new OpenGLIndexBuffer(indices, size);
            case RenderAPI::Vulkan : return nullptr;
        }

        return nullptr;
    }

}