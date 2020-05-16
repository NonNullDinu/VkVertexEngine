#pragma once

#include "Core/Core.h"
#include "UniformPack.h"

namespace Vertex
{
    class Shader
    {
    public:
        UniformPack uniforms;
        virtual ~Shader() { }

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void BeforeRender() = 0;
        virtual void AfterRender() = 0;

        template <size_t InputBindingLen, size_t InputAttribLen>
        static Shader* Create(const std::vector<unsigned char>& vertex_src,
            const std::vector<unsigned char>&                   fragment_src,
            std::tuple<std::array<VkVertexInputBindingDescription, InputBindingLen>,
                std::array<VkVertexInputAttributeDescription, InputAttribLen>>
                vertex_shader_input_layout);
    };

}