#pragma once

#include "Core/Core.h"

namespace Vertex {

    class Shader
    {
    public:
        virtual ~Shader() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Shader* Create(const char* vertex_src, const char* fragment_src);
    };

}

#include "GL/OpenGL/OpenGLShader.h"
// ... per rendering API