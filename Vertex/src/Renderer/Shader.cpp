#include "Shader.h"

namespace Vertex {

    Shader* Shader::Create(const char* vertex_src, const char* fragment_src)
    {
        switch(Renderer::GetRenderAPI())
        {
            case RenderAPI::OpenGL : return new OpenGLShader(vertex_src, fragment_src);
            case RenderAPI::Vulkan : return nullptr;
        }

        return nullptr;
    }

}