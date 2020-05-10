#pragma once

#include "GL/Shader.h"

namespace Vertex
{

    class VulkanShaderPipeline : public Shader
    {
    public:
        VulkanShaderPipeline(const std::vector<unsigned char>& vertex_src,
                             const std::vector<unsigned char>& fragment_src);
        ~VulkanShaderPipeline();
        void Bind() const override;
        void Unbind() const override;

        inline void SetVkCommandBuffer(VkCommandBuffer command_buffer)
        { m_CommandBuffer = command_buffer; }

        void CleanUp();
    private:
        VkPipeline m_GraphicsPipeline;
        VkCommandBuffer m_CommandBuffer;
    };

}