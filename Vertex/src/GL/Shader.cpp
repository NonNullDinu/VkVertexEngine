#include "Shader.h"

#if defined(VX_RENDER_API_OPENGL)
    #include "OpenGL/OpenGLShader.h"
#elif defined(VX_RENDER_API_VULKAN)
    #include "Vulkan/VulkanShaderPipeline.h"
#endif
// ... per rendering API

namespace Vertex
{

    // helper functions
    const size_t GetSizeOfShaderDataType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Bool   :
            return 1;
        case ShaderDataType::Mat3   :
            return 36;
        case ShaderDataType::Mat4   :
            return 64;
        case ShaderDataType::Int    :
            return 4;
        case ShaderDataType::Int2   :
            return 8;
        case ShaderDataType::Int3   :
            return 12;
        case ShaderDataType::Int4   :
            return 16;
        case ShaderDataType::Float  :
            return 4;
        case ShaderDataType::Float2 :
            return 8;
        case ShaderDataType::Float3 :
            return 12;
        case ShaderDataType::Float4 :
            return 16;
        }

        CoreLogger::Get()->error("Unknown shader data type, cancelling...");
        return 0;
    };

    const uint32_t GetComponentCountOfShaderDataType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Bool   :
            return 1;
        case ShaderDataType::Mat3   :
            return 9;
        case ShaderDataType::Mat4   :
            return 16;
        case ShaderDataType::Int    :
            return 1;
        case ShaderDataType::Int2   :
            return 2;
        case ShaderDataType::Int3   :
            return 3;
        case ShaderDataType::Int4   :
            return 4;
        case ShaderDataType::Float  :
            return 1;
        case ShaderDataType::Float2 :
            return 2;
        case ShaderDataType::Float3 :
            return 3;
        case ShaderDataType::Float4 :
            return 4;
        }

        CoreLogger::Get()->error("Unknown shader data type, cancelling...");
        return 0;
    }

#ifdef VX_RENDER_API_VULKAN
    template<size_t InputBindingLen, size_t InputAttribLen>
#endif
    Shader* Shader::Create(const std::vector<unsigned char>& vertex_src,
                           const std::vector<unsigned char>& fragment_src
#ifdef VX_RENDER_API_VULKAN
        , std::tuple<std::array<VkVertexInputBindingDescription, InputBindingLen>,
                     std::array<VkVertexInputAttributeDescription, InputAttribLen>>
                           vertex_shader_input_layout
#endif
    )
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLShader(vertex_src, fragment_src);
#elif defined(VX_RENDER_API_VULKAN)
        return new VulkanShaderPipeline
            (vertex_src,
             fragment_src,
             std::get<0>(vertex_shader_input_layout),
             std::get<1>(vertex_shader_input_layout)
            );
#else
        return nullptr; // for now
#endif
    }

    #ifdef VX_RENDER_API_VULKAN
    template
    Shader* Shader::Create<2, 2>(const std::vector<unsigned char>& vertex_src,
                                 const std::vector<unsigned char>& fragment_src,
                                 std::tuple<std::array<VkVertexInputBindingDescription,
                                                       2>,
                                            std::array<VkVertexInputAttributeDescription,
                                                       2>>
                                 vertex_shader_input_layout
    );
    #endif
}