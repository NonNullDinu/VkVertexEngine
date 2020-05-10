#include "Renderer.h"

#if defined(VX_RENDER_API_OPENGL)
    #include "GL/OpenGL/OpenGLShader.h"
#endif

namespace Vertex
{
    
    GraphicsAPI* Renderer::s_GraphicsAPI = GraphicsAPI::Create();
    Scene* Renderer::s_Scene = nullptr;

    void Renderer::BeginScene(Camera& camera)
    {
        s_Scene = new Scene(camera);
    }

    void Renderer::Submit(std::shared_ptr<VertexArray>& vertex_array, const std::shared_ptr<Shader>& shader)
    {
        shader->Bind();
#if defined(VX_RENDER_API_OPENGL)
        (*std::dynamic_pointer_cast<OpenGLShader>(shader))["u_ProjectionViewMatrix"] = s_Scene->camera.GetProjectionViewMatrix();
#endif
        vertex_array->Bind();
        s_GraphicsAPI->DrawIndexed(vertex_array);
    }

    void Renderer::EndScene()
    {
    }

    RenderAPI Renderer::GetAPI()
    {
#if defined(VX_RENDER_API_OPENGL)
        return RenderAPI::OpenGL;
#elif defined(VX_RENDER_API_VULKAN)
        return RenderAPI::Vulkan;
#else
        return nullptr;
#endif
    }

}
