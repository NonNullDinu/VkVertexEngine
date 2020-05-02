#pragma once

#include <Renderer/Shader.h>
namespace Vertex {
    class VulkanShaderPipeline : public Shader {
    public:
        VulkanShaderPipeline(const std::vector<char> vertex_src, const std::vector<char> fragment_src);
        ~VulkanShaderPipeline();
        void Bind() const override;
        void Unbind() const override;

        inline void SetVkCommandBuffer(VkCommandBuffer command_buffer) {m_CommandBuffer = command_buffer;}

    private:
        VkPipelineLayout m_PipelineLayout;
        VkPipeline m_GraphicsPipeline;
        VkCommandBuffer m_CommandBuffer;
    };
}