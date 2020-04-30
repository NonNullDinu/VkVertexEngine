#pragma once
namespace Vertex {
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

    template <typename T>
    constexpr bool full_of() { return true; }
    template <typename T, typename First, typename... Rest>
    constexpr bool full_of()
    { return std::is_same_v<T, First> && full_of<T, Rest...>(); }

    template<typename ...args>
    struct VulkanBufferBindings {
        static std::array<VkVertexInputAttributeDescription, sizeof...(args)> getAttributeDescriptions() {
            VX_CORE_STATIC_ASSERT(full_of<VulkanBufferBinding, args...>, "not all args are of type VulkanBufferBinding");
            std::array<VkVertexInputAttributeDescription, sizeof...(args)> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);
            return attributeDescriptions;
        }
    };
}