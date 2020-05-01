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
    struct is_VulkanBufferBinding : std::false_type {};

    template <typename T, int loc>
    struct is_VulkanBufferBinding<VulkanBufferBinding<T, loc>> : std::true_type {};

    template <typename T>
    constexpr bool VulkanBufferBindingTest() { return true; }
    template <typename T, typename First, typename... Rest>
    constexpr bool VulkanBufferBindingTest()
    { return is_VulkanBufferBinding<First>::value && VulkanBufferBindingTest<T, Rest...>(); }

    template<typename ...args>
    struct VulkanBufferBindings {
        static std::array<VkVertexInputAttributeDescription, sizeof...(args)> getAttributeDescriptions() {
            VX_CORE_STATIC_ASSERT(VulkanBufferBindingTest<std::nullptr_t, args...>(), "not all args are of type VulkanBufferBinding");
            std::array<VkVertexInputAttributeDescription, sizeof...(args)> attributeDescriptions{};
//            attributeDescriptions[0].binding = 0;
//            attributeDescriptions[0].location = 0;
//            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
//            attributeDescriptions[0].offset = offsetof(Vertex, pos);
            return attributeDescriptions;
        }
    };
}