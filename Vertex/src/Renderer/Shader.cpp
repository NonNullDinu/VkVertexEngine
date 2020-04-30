#include "Shader.h"

namespace Vertex {

    Shader* Shader::Create(const char* vertex_src, const char* fragment_src)
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLShader(vertex_src, fragment_src);
#else
        return nullptr;
#endif
    }

}