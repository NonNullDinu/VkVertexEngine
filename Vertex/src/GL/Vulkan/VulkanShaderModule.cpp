#include "VulkanShaderModule.h"
#include "VulkanContext.h"

namespace Vertex
{

    VulkanShaderModule::VulkanShaderModule(const std::vector<char> source,
                                           VkPipelineShaderStageCreateInfo shader_stage_info)
    {
        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = source.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(source.data());

        if (vkCreateShaderModule(VulkanContext::GetContext()->GetDevice(),
                                 &create_info,
                                 nullptr,
                                 &m_InternalVkShaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module!");
        }

        shader_stage_info.module = m_InternalVkShaderModule;

        vkDestroyShaderModule(VulkanContext::GetContext()->GetDevice(), m_InternalVkShaderModule, nullptr);
    }

}