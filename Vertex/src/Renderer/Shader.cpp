#include "Shader.h"

#include "GL/OpenGL/OpenGLShader.h"

namespace Vertex {

    Shader* Shader::Create(const char* vertex_src, const char* fragment_src)
    {
        return new OpenGLShader(vertex_src, fragment_src);
    }

}