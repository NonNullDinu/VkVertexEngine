#pragma once

namespace Vertex
{
    template<typename T, int Binding, int Location, int Offset = 0, VkFormat Format = VK_FORMAT_R32G32_SFLOAT>
    struct VulkanBufferBinding
    {
        static constexpr VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription binding_description{};
            binding_description.binding = Location;
            binding_description.stride = sizeof(T);
            binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return binding_description;
        }

        static constexpr size_t GetSize()
        { return sizeof(T); }

        static constexpr int GetLocation()
        { return Location; }

        static constexpr int GetBinding()
        { return Binding; }

        static constexpr int GetOffset()
        { return Offset; }

        static constexpr VkFormat GetFormat()
        { return Format; }
    };

    template<typename T>
    struct is_VulkanBufferBinding : std::false_type
    {
    };

    template<typename T, int Binding, int Location, int Offset, VkFormat Format>
    struct is_VulkanBufferBinding<VulkanBufferBinding<T, Binding, Location, Offset, Format> > : std::true_type
    {
    };

    VX_TEMPLATE_ALL(VulkanBufferBindingTest, is_VulkanBufferBinding)

    template<int I, int Total_Len>
    constexpr void GenerateArray(std::array<VkVertexInputAttributeDescription, Total_Len> arr)
    {}
    template<int I, int Total_Len, typename First, typename... Rest>
    constexpr void GenerateArray(std::array<VkVertexInputAttributeDescription, Total_Len> arr)
    {
        arr[I].binding = First::GetBinding();
        arr[I].location = First::GetLocation();
        arr[I].format = First::GetFormat();
        arr[I].offset = First::GetOffset();
        GenerateArray<I + 1, Total_Len, Rest...>(arr);
    }

    template<typename ...args>
    struct VulkanBufferBindings
    {
        static constexpr std::array<VkVertexInputAttributeDescription, sizeof...(args)> GetAttributeDescriptions()
        {
            VX_CORE_STATIC_ASSERT(VX_TEMPLATE_TCALL(VulkanBufferBindingTest, args...),
                                  "not all args are of type VulkanBufferBinding");
            std::array<VkVertexInputAttributeDescription, sizeof...(args)> attribute_descriptions{};
            GenerateArray<0, sizeof...(args), args...>(attribute_descriptions);
            return attribute_descriptions;
        }
    };

}