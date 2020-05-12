#include "GraphicsAPI.h"

#if defined(VX_RENDER_API_OPENGL)
    #include "OpenGL/OpenGLGraphicsAPI.h"
#elif defined(VX_RENDER_API_VULKAN)
    #include "Vulkan/VulkanGraphicsAPI.h"
#endif

namespace Vertex
{

    GraphicsAPI* GraphicsAPI::Create()
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLGraphicsAPI();
#elif defined(VX_RENDER_API_VULKAN)
        return new VulkanGraphicsAPI();
#else
        return nullptr; // for now
#endif
    }
    template <GraphicsAPIAction action, typename... GraphicsAPIActionArgument>
    void GraphicsAPI::Queue(GraphicsAPIActionArgument... args)
    {
        if (action == GraphicsAPIAction::DrawIndexed)
        {
            DrawIndexed(std::get<0>(std::forward_as_tuple(args...)));
        }
    }

    template void GraphicsAPI::Queue<DrawIndexed, std::shared_ptr<VertexArray>>(std::shared_ptr<VertexArray> vertex_array);
}