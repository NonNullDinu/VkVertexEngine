#include "Renderer.h"

namespace Vertex
{

    GraphicsAPI* Renderer::s_GraphicsAPI = GraphicsAPI::Create();
    Scene*       Renderer::s_Scene = nullptr;

    void Renderer::BeginScene(Camera& camera) { s_Scene = new Scene(camera); }

    void Renderer::Submit(std::shared_ptr<VertexArray>& vertex_array, const std::shared_ptr<Shader>& shader)
    {
        shader->Bind();
        vertex_array->Bind();
        s_GraphicsAPI->Queue<DrawIndexed>(vertex_array);
    }

    void Renderer::EndScene() { }

    RenderAPI Renderer::GetAPI() { return RenderAPI::Vulkan; }

}
