#include "VulkanGraphicsAPI.h"
#include "VulkanContext.h"

namespace Vertex
{

    void VulkanGraphicsAPI::Clear() { }
    void VulkanGraphicsAPI::Clear(const glm::vec3& color) { }
    void VulkanGraphicsAPI::Clear(const glm::vec4& color) { }
    void VulkanGraphicsAPI::DrawIndexed(std::shared_ptr<VertexArray> vertex_array)
    {
        vkCmdDrawIndexed(VulkanContext::GetContext()->GetCurrentCommandBuffer(),
            static_cast<uint32_t>(vertex_array->GetIndexBuffer()->GetCount()), 1, 0, 0, 0);
    }
}