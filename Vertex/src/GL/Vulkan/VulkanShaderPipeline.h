#pragma once

#include <Renderer/Shader.h>
namespace Vertex {
    class VulkanShaderPipeline : public Shader {
    public:
        VulkanShaderPipeline(const std::vector<char> vertex_src, const std::vector<char> fragment_src);
        ~VulkanShaderPipeline();
        void Bind() const override;
        void Unbind() const override;

    private:
        VkPipelineLayout m_PipelineLayout;
        VkPipeline m_GraphicsPipeline;
    };
}