#pragma once

namespace Vertex
{

    template<typename T, int location>
    struct VulkanBufferBinding
    {
        static VkVertexInputBindingDescription getBindingDescription()
        {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = location;
            bindingDescription.stride = sizeof(T);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }
    };

    template<typename T>
    struct is_VulkanBufferBinding : std::false_type
    {
    };

    template<typename T, int loc>
    struct is_VulkanBufferBinding<VulkanBufferBinding<T, loc> > : std::true_type
    {
    };

    VX_TEMPLATE_ALL(VulkanBufferBindingTest, is_VulkanBufferBinding)

    template<typename ...args>
    struct VulkanBufferBindings
    {
        static constexpr std::array<VkVertexInputAttributeDescription, sizeof...(args)> getAttributeDescriptions()
        {
            VX_CORE_STATIC_ASSERT(VX_TEMPLATE_TCALL(VulkanBufferBindingTest, args...),
                                  "not all args are of type VulkanBufferBinding");
            std::array<VkVertexInputAttributeDescription, sizeof...(args)> attributeDescriptions{};
//            attributeDescriptions[0].binding = 0;
//            attributeDescriptions[0].location = 0;
//            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
//            attributeDescriptions[0].offset = offsetof(Vertex, pos);
            return attributeDescriptions;
        }
    };

}