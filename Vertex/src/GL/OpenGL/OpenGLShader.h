#pragma once

#include "Core/Core.h"

#include "Renderer/Shader.h"
#include "OpenGLUniformPack.h"

namespace Vertex {

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const char* vertex_src, const char* fragment_src);
        ~OpenGLShader();

        void StartLoadingUniformsToPack();
        void LoadToUniformToPack(std::string uniform_var_name);
        void StopLoadingUniformsToPack();

        OpenGLUniform& operator[](std::string name);

        void Bind() const override;
        void Unbind() const override;

    private:
        uint32_t m_ID;
        OpenGLUniformPack m_UniformPack;
    };

}