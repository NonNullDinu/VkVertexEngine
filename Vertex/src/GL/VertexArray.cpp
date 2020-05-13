#include "VertexArray.h"

#include "Vulkan/VulkanVertexArray.h"

namespace Vertex
{

    VertexArray* VertexArray::Create() { return new VulkanVertexArray(); }

}