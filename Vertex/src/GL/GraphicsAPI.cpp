#include "GraphicsAPI.h"

#include "Vulkan/VulkanGraphicsAPI.h"

namespace Vertex
{

    GraphicsAPI* GraphicsAPI::Create() { return new VulkanGraphicsAPI(); }
    template <GraphicsAPIAction action, typename... GraphicsAPIActionArgument>
    void GraphicsAPI::Queue(GraphicsAPIActionArgument... args)
    {
        if (action == GraphicsAPIAction::DrawIndexed)
        {
            DrawIndexed(std::get<0>(std::forward_as_tuple(args...)));
        }
    }

    template void GraphicsAPI::Queue<DrawIndexed, std::shared_ptr<VertexArray>>(
        std::shared_ptr<VertexArray> vertex_array);
}