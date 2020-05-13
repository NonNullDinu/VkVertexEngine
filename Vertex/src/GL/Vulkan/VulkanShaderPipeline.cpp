#include "VulkanShaderPipeline.h"

namespace Vertex
{
    VulkanShaderPipeline::~VulkanShaderPipeline() { CleanUp(); }

    void VulkanShaderPipeline::Bind() const
    {
        vkCmdBindPipeline(VulkanContext::GetContext()->GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS,
            m_GraphicsPipeline);
        vkCmdBindDescriptorSets(VulkanContext::GetContext()->GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS,
            VulkanContext::GetContext()->GetPipelineLayout(), 0, 1,
            VulkanContext::GetContext()->GetCurrentDescriptorSet(), 0, nullptr);
    }
    void VulkanShaderPipeline::Unbind() const { }
    void VulkanShaderPipeline::CleanUp()
    {
        vkDestroyPipeline(VulkanContext::GetContext()->GetDevice(), m_GraphicsPipeline, nullptr);
    }
}