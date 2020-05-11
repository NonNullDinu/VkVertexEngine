#pragma once

namespace Vertex
{

    class VulkanShaderModule
    {
        friend class VulkanShaderPipeline;

    public:
        VulkanShaderModule(const std::vector<unsigned char>& source,
            VkPipelineShaderStageCreateInfo&                 shader_stage_info);
        ~VulkanShaderModule();

    private:
        VkShaderModule m_InternalVkShaderModule;
    };

}