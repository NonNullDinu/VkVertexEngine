#include "GraphicsContext.h"

#include "Vulkan/VulkanContext.h"

namespace Vertex
{

    GraphicsContext* GraphicsContext::Create(GLFWwindow* window_handle) { return new VulkanContext(window_handle); }

}