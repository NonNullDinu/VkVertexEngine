#include "Renderer.h"

namespace Vertex {
    
    GraphicsAPI* Renderer::s_GraphicsAPI = GraphicsAPI::Create();

    void Renderer::Submit(std::shared_ptr<VertexArray> vertex_array)
    {
        vertex_array->Bind();
        s_GraphicsAPI->DrawIndexed(vertex_array);
    }

    void Renderer::BeginScene()
    {
    }

    void Renderer::EndScene()
    {
    }

    RenderAPI Renderer::GetAPI()
    {
#if defined(VX_RENDER_API_OPENGL)
        return RenderAPI::OpenGL;
#elif defined(VX_RENDER_API_VULKAN)
        return RednderAPI::Vulkan;
#else
        return nullptr;
#endif
    }

}
