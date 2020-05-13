#pragma once

#include "GL/Vulkan/VulkanShaderPipeline.h"
#include "Shader.h"

namespace Vertex
{
    template <size_t InputBindingLen, size_t InputAttribLen>
    Shader* Shader::Create(const std::vector<unsigned char>& vertex_src, const std::vector<unsigned char>& fragment_src,
        std::tuple<std::array<VkVertexInputBindingDescription, InputBindingLen>,
            std::array<VkVertexInputAttributeDescription, InputAttribLen>>
            vertex_shader_input_layout)
    {
        return new VulkanShaderPipeline(
            vertex_src, fragment_src, std::get<0>(vertex_shader_input_layout), std::get<1>(vertex_shader_input_layout));
    }
}