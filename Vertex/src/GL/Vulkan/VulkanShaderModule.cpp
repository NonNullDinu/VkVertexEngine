#include "VulkanShaderModule.h"
#include "VulkanContext.h"

namespace Vertex {

    VulkanShaderModule::VulkanShaderModule(const std::vector<char> source, VkPipelineShaderStageCreateInfo shader_stage_info)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = source.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());

        if (vkCreateShaderModule(VulkanContext::GetContext()->GetDevice(), &createInfo, nullptr, &m_InternalVkShaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        shader_stage_info.module = m_InternalVkShaderModule;

        vkDestroyShaderModule(VulkanContext::GetContext()->GetDevice(), m_InternalVkShaderModule, nullptr);
    }
}