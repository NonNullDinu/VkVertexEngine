#include "Shader.h"

namespace Vertex {

    // helper functions
    size_t GetSizeOfShaderDataType(ShaderDataType type)
    {
        switch(type)
        {
            case ShaderDataType::Bool   : return 1;
            case ShaderDataType::Mat3   : return 36;
            case ShaderDataType::Mat4   : return 64;
            case ShaderDataType::Int    : return 4;
            case ShaderDataType::Int2   : return 8;
            case ShaderDataType::Int3   : return 12;
            case ShaderDataType::Int4   : return 16;
            case ShaderDataType::Float  : return 4;
            case ShaderDataType::Float2 : return 8;
            case ShaderDataType::Float3 : return 12;
            case ShaderDataType::Float4 : return 16;
        }

        Logger::GetCoreLogger()->error("Unknown shader data type, cancelling...");
        return 0;
    };

    uint32_t GetComponentCountOfShaderDataType(ShaderDataType type)
    {
        switch(type)
        {
            case ShaderDataType::Bool   : return 1;
            case ShaderDataType::Mat3   : return 9;
            case ShaderDataType::Mat4   : return 16;
            case ShaderDataType::Int    : return 1;
            case ShaderDataType::Int2   : return 2;
            case ShaderDataType::Int3   : return 3;
            case ShaderDataType::Int4   : return 4;
            case ShaderDataType::Float  : return 1;
            case ShaderDataType::Float2 : return 2;
            case ShaderDataType::Float3 : return 3;
            case ShaderDataType::Float4 : return 4;
        }

        Logger::GetCoreLogger()->error("Unknown shader data type, cancelling...");
        return 0;
    }


    Shader* Shader::Create(const char* vertex_src, const char* fragment_src)
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLShader(vertex_src, fragment_src);
#else
        return nullptr;
#endif
    }

}