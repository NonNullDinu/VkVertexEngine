#pragma once

namespace Vertex
{
    template <typename T, int Binding, int Location, int Offset = 0, VkFormat Format = VK_FORMAT_R32G32_SFLOAT, bool HasBindingDescription = true>
    struct VulkanBufferBinding
    {
        static constexpr VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription binding_description {};
            binding_description.binding = Binding;
            binding_description.stride = sizeof(T);
            binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return binding_description;
        }

        static constexpr VkVertexInputAttributeDescription GetAttributeDescription()
        {
            VkVertexInputAttributeDescription attrbute_description {};
            attrbute_description.binding = Binding;
            attrbute_description.location = Location;
            attrbute_description.format = Format;
            attrbute_description.offset = Offset;
            return attrbute_description;
        }

        static constexpr size_t GetSize()
        {
            return sizeof(T);
        }

        static constexpr int GetLocation()
        {
            return Location;
        }

        static constexpr int GetBinding()
        {
            return Binding;
        }

        static constexpr int GetOffset()
        {
            return Offset;
        }

        static constexpr VkFormat GetFormat()
        {
            return Format;
        }

        static constexpr bool GetHasBindingDescription()
        {
            return HasBindingDescription;
        }
    };

    template <typename T>
    struct is_VulkanBufferBinding : std::false_type
    {
    };

    template <typename T, int Binding, int Location, int Offset, VkFormat Format, bool HasBindingDescription>
    struct is_VulkanBufferBinding<VulkanBufferBinding<T, Binding, Location, Offset, Format, HasBindingDescription>> : std::true_type
    {
    };

    VX_TEMPLATE_ALL(VulkanBufferBindingTest, is_VulkanBufferBinding)

    template <int I, int J, int Total_Len, int Total_LenBindings>
    constexpr void GenerateArray(std::array<VkVertexInputBindingDescription, Total_LenBindings>& binding_arr,
        std::array<VkVertexInputAttributeDescription, Total_Len>&                                arr)
    {
    }
    template <int I, int J, int Total_Len, int Total_LenBindings, typename First, typename... Rest>
    constexpr void GenerateArray(std::array<VkVertexInputBindingDescription, Total_LenBindings>& binding_arr,
        std::array<VkVertexInputAttributeDescription, Total_Len>&                                arr)
    {
        constexpr bool has_binding_description = First::GetHasBindingDescription();
        arr[I] = First::GetAttributeDescription();
        if (has_binding_description)
            binding_arr[J] = First::GetBindingDescription();
        GenerateArray<I + 1, J + (has_binding_description ? 1 : 0), Total_Len, Total_LenBindings, Rest...>(binding_arr, arr);
    }

    template <int VertexBuffers, typename... args>
    struct VulkanBufferBindings
    {
        static constexpr std::tuple<const std::array<VkVertexInputBindingDescription, VertexBuffers>,
            const std::array<VkVertexInputAttributeDescription,
                sizeof...(args)>>
        GetAttributeDescriptions()
        {
            VX_CORE_STATIC_ASSERT(VX_TEMPLATE_TCALL(VulkanBufferBindingTest, args...),
                "not all args are of type VulkanBufferBinding");
            std::array<VkVertexInputBindingDescription, VertexBuffers>     binding_descriptions {};
            std::array<VkVertexInputAttributeDescription, sizeof...(args)> attribute_descriptions {};
            GenerateArray<0, 0, sizeof...(args), VertexBuffers, args...>(binding_descriptions, attribute_descriptions);
            return std::make_tuple(binding_descriptions, attribute_descriptions);
        }
    };

}