#include "Shader.h"

#if VX_RENDER_API == VX_RENDER_API_OPENGL
#include "GL/OpenGL/OpenGLShader.h"
#endif

namespace Vertex {

    Shader* Shader::Create(const char* vertex_src, const char* fragment_src)
    {
#if VX_RENDER_API == VX_RENDER_API_OPENGL
        return new OpenGLShader(vertex_src, fragment_src);
#elif VX_RENDER_API == VX_RENDER_API_VULKAN
        return nullptr;
#endif
    }

}