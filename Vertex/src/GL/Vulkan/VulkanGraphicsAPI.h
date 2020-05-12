#pragma once

#include "GL/GraphicsAPI.h"

namespace Vertex
{
    class VulkanGraphicsAPI : public GraphicsAPI
    {
        void Clear() override;
        void Clear(const glm::vec3& color) override;
        void Clear(const glm::vec4& color) override;

        void DrawIndexed(std::shared_ptr<VertexArray> vertex_array) override;
    };
}