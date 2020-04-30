#pragma once

#include "Core/Core.h"

namespace Vertex {

    // Make definite shader types so the API is more consistent
    enum class ShaderDataType
    {
        Bool,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Float, Float2, Float3, Float4
    };

    // helper function
    uint32_t GetSizeOfShaderDataType(ShaderDataType type);

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