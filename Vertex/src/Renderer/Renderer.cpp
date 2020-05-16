#include "Renderer.h"

namespace Vertex
{

    GraphicsAPI* Renderer::s_GraphicsAPI = GraphicsAPI::Create();
    Scene*       Renderer::s_Scene = nullptr;

    void Renderer::BeginScene(Camera& camera) { s_Scene = new Scene(camera); }

    void Renderer::Submit(std::shared_ptr<VertexArray>& vertex_array, const std::shared_ptr<Shader>& shader)
    {
        shader->BeforeRender();
        vertex_array->BeforeRender();
        glm::mat4 vpm = s_Scene->camera.GetProjectionViewMatrix();

        s_GraphicsAPI->DrawIndexed(vertex_array);
        vertex_array->AfterRender();
        shader->AfterRender();
    }

    void Renderer::EndScene() { }

    RenderAPI Renderer::GetAPI() { return RenderAPI::Vulkan; }

}
