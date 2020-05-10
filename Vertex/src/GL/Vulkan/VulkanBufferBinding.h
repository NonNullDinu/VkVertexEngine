#pragma once

namespace Vertex
{

    template<typename T, int location>
    struct VulkanBufferBinding
    {
        static VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription binding_description{};
            binding_description.binding = location;
            binding_description.stride = sizeof(T);
            binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return binding_description;
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
        static constexpr std::array<VkVertexInputAttributeDescription, sizeof...(args)> GetAttributeDescriptions()
        {
            VX_CORE_STATIC_ASSERT(VX_TEMPLATE_TCALL(VulkanBufferBindingTest, args...),
                                  "not all args are of type VulkanBufferBinding");
            std::array<VkVertexInputAttributeDescription, sizeof...(args)> attribute_descriptions{};
//            attribute_descriptions[0].binding = 0;
//            attribute_descriptions[0].location = 0;
//            attribute_descriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
//            attribute_descriptions[0].offset = offsetof(Vertex, pos);
            return attribute_descriptions;
        }
    };

}