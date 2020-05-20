#pragma once

namespace Vertex
{
    template <typename T> static constexpr VkFormat DeductFormat()
    {
        if constexpr (std::is_same_v<T, glm::vec1> || std::is_same_v<T, float>)
        {
            return VK_FORMAT_R32_SFLOAT;
        }
        else if constexpr (std::is_same_v<T, glm::vec2>)
        {
            return VK_FORMAT_R32G32_SFLOAT;
        }
        else if constexpr (std::is_same_v<T, glm::vec3>)
        {
            return VK_FORMAT_R32G32B32_SFLOAT;
        }
        else if constexpr (std::is_same_v<T, glm::vec4>)
        {
            return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        else if constexpr (std::is_same_v<T, glm::dvec1> || std::is_same_v<T, double>)
        {
            return VK_FORMAT_R64_SFLOAT;
        }
        else if constexpr (std::is_same_v<T, glm::dvec2>)
        {
            return VK_FORMAT_R64G64_SFLOAT;
        }
        else if constexpr (std::is_same_v<T, glm::dvec3>)
        {
            return VK_FORMAT_R64G64B64_SFLOAT;
        }
        else if constexpr (std::is_same_v<T, glm::dvec4>)
        {
            return VK_FORMAT_R64G64B64A64_SFLOAT;
        }
    }

    template <typename T, int Offset, int Location, int Binding = 0, VkFormat Format = DeductFormat<T>()>
    class VertexField : public T
    {
    public:
        static constexpr VkVertexInputAttributeDescription GetAttributeDescription()
        {
            VkVertexInputAttributeDescription attribute_description {};
            attribute_description.binding = Binding;
            attribute_description.location = Location;
            attribute_description.format = Format;
            attribute_description.offset = Offset;
            return attribute_description;
        }

        template <typename... Types> VertexField(Types... values) : T(values...) { }
    };

    template <typename> struct IsVertexFieldType : public std::false_type
    {
    };
    template <typename T, int Offset, int Location, int Binding, VkFormat Format>
    struct IsVertexFieldType<VertexField<T, Offset, Location, Binding, Format>> : public std::true_type
    {
    };

    template <typename T> inline constexpr bool IsVertexField = IsVertexFieldType<T>::value;

    template <VxSize Binding, VxSize Stride> struct VxDataBuffer
    {
        static constexpr VkVertexInputBindingDescription GetBindingDescription()
        {
            VkVertexInputBindingDescription binding_description {};
            binding_description.binding = Binding;
            binding_description.stride = Stride;
            binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return binding_description;
        }
    };

    // https://stackoverflow.com/a/34672753/12576629
    template <template <VxSize Binding, VxSize Stride> class base, typename derived> struct is_base_of_template_impl
    {
        template <VxSize Binding, VxSize Stride> static constexpr std::true_type test(const base<Binding, Stride>*);
        static constexpr std::false_type                                         test(...);
        using type = decltype(test(std::declval<derived*>()));
    };

    template <template <VxSize, VxSize> class base, typename derived>
    using is_base_of_template = typename is_base_of_template_impl<base, derived>::type;

    template <typename T> inline constexpr bool IsBuffer = is_base_of_template<VxDataBuffer, T>::value;

    template <typename> constexpr VxSize                                     CountBuffers() { return 0; }
    template <typename T, typename First, typename... Rest> constexpr VxSize CountBuffers()
    {
        return (IsBuffer<First> ? 1 : 0) + CountBuffers<T, Rest...>();
    }

    template <int I, int J, int VertexBuffers, int VertexInputFieldsCount>
    constexpr void GenerateArrays(std::array<VkVertexInputBindingDescription, VertexBuffers>& bindings,
        std::array<VkVertexInputAttributeDescription, VertexInputFieldsCount>&                attributes)
    {
        VX_CORE_STATIC_ASSERT(I == VertexBuffers && J == VertexInputFieldsCount, "something went wrong");
    }
    template <int I, int J, int VertexBuffers, int VertexInputFieldsCount, typename First, typename... Rest>
    constexpr void GenerateArrays(std::array<VkVertexInputBindingDescription, VertexBuffers>& bindings,
        std::array<VkVertexInputAttributeDescription, VertexInputFieldsCount>&                attributes)
    {
        VX_CORE_STATIC_ASSERT(IsBuffer<First> || IsVertexField<First>,
            "input types of VulkanVertexShaderInputs should be either children classes of VxDataBuffer or template "
            "specializations of VxDataBuffer");
        if constexpr (IsBuffer<First>)
        {
            bindings[I] = First::GetBindingDescription();
            GenerateArrays<I + 1, J, VertexBuffers, VertexInputFieldsCount, Rest...>(bindings, attributes);
        }
        if constexpr (IsVertexField<First>)
        {
            attributes[J] = First::GetAttributeDescription();
            GenerateArrays<I, J + 1, VertexBuffers, VertexInputFieldsCount, Rest...>(bindings, attributes);
        }
    }

    template <typename... Args> struct VulkanVertexShaderInputs
    {
    private:
        static constexpr VxSize s_VertexBuffers = CountBuffers<std::nullptr_t, Args...>();
        static constexpr VxSize s_VertexFieldsCount = sizeof...(Args) - s_VertexBuffers;

    public:
        static constexpr auto GetAttributeDescriptions()
        {
            std::array<VkVertexInputBindingDescription, s_VertexBuffers>       binding_descriptions {};
            std::array<VkVertexInputAttributeDescription, s_VertexFieldsCount> attribute_descriptions {};
            GenerateArrays<0, 0, s_VertexBuffers, s_VertexFieldsCount, Args...>(
                binding_descriptions, attribute_descriptions);
            return std::make_tuple(binding_descriptions, attribute_descriptions);
        }
        static constexpr auto GetVertexArrayLayout() { }
    };
}