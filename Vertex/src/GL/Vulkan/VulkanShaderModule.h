#pragma once

namespace Vertex
{

    class VulkanShaderModule
    {
    public:
        VulkanShaderModule(const std::vector<char> source, VkPipelineShaderStageCreateInfo shader_stage_info);

    private:
        VkShaderModule m_InternalVkShaderModule;
    };

}