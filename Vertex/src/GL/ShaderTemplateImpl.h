#pragma once

#include "GL/Vulkan/VulkanShaderPipeline.h"
#include "Shader.h"

namespace Vertex
{
    template <typename> struct is_tuple : std::false_type
    {
    };

    template <typename... T> struct is_tuple<std::tuple<T...>> : std::true_type
    {
    };

    template <typename TupleType>
    Shader* Shader::Create(const std::vector<unsigned char>& vertex_src, const std::vector<unsigned char>& fragment_src,
        TupleType vertex_shader_input_layout)
    {
        VX_CORE_STATIC_ASSERT(is_tuple<TupleType>::value, "Expected tuple type for third argument of Vertex::Shader::Create");
        return new VulkanShaderPipeline(
            vertex_src, fragment_src, std::get<0>(vertex_shader_input_layout), std::get<1>(vertex_shader_input_layout));
    }
}