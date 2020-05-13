#pragma once

#include "Core/Core.h"

namespace Vertex
{
    // Make definite shader types so the API is more consistent
    enum class ShaderDataType
    {
        Bool,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Float,
        Float2,
        Float3,
        Float4
    };

    // helper functions
    const size_t   GetSizeOfShaderDataType(ShaderDataType type);
    const uint32_t GetComponentCountOfShaderDataType(ShaderDataType type);

    class Shader
    {
    public:
        virtual ~Shader() { }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        template <size_t InputBindingLen, size_t InputAttribLen>
        static Shader* Create(const std::vector<unsigned char>& vertex_src,
            const std::vector<unsigned char>&                   fragment_src,
            std::tuple<std::array<VkVertexInputBindingDescription, InputBindingLen>,
                std::array<VkVertexInputAttributeDescription, InputAttribLen>>
                vertex_shader_input_layout);
    };

}