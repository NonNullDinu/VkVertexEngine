#pragma once

#include "Core/Core.h"

#include "Renderer/Shader.h"

namespace Vertex {

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const char* vertex_src, const char* fragment_src);
        ~OpenGLShader();

        void Bind() const override;
        void Unbind() const override;

    private:
        uint32_t m_ID;
    };

}