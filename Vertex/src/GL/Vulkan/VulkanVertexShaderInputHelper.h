#pragma once

#include "VulkanVertexArrayLayout.h"
namespace Vertex
{
    namespace VertexShaderInputUtils
    {
        template <typename T> static constexpr VkFormat DeduceFormat()
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
    }

    template <typename T, int Offset, int Location, VkFormat Format = VertexShaderInputUtils::DeduceFormat<T>()>
    struct VertexField : public T
    {
        static constexpr VkVertexInputAttributeDescription GetAttributeDescription(int binding)
        {
            VkVertexInputAttributeDescription attribute_description {};
            attribute_description.binding = binding;
            attribute_description.location = Location;
            attribute_description.format = Format;
            attribute_description.offset = Offset;
            return attribute_description;
        }

        template <typename... Types> VertexField(Types... values) : T(values...) { }
    };

    namespace VertexShaderInputUtils
    {
        template <typename> struct IsVertexFieldType : public std::false_type
        {
        };
        template <typename T, int Offset, int Location, VkFormat Format>
        struct IsVertexFieldType<VertexField<T, Offset, Location, Format>> : public std::true_type
        {
        };

        template <typename T> inline constexpr bool IsVertexField = IsVertexFieldType<T>::value;
    }

    enum VxDataBufferType
    {
        STATIC,
        DYNAMIC
    };

    template <VxDataBufferType type, VxSize Binding, VxSize Offset = 0> struct VxDataBuffer
    {
        static constexpr VkVertexInputBindingDescription GetBindingDescription(VxSize struct_size)
        {
            VkVertexInputBindingDescription binding_description {};
            binding_description.binding = Binding;
            binding_description.stride = struct_size;
            binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return binding_description;
        }

        static constexpr VxDataBufferType GetDataBufferType() { return type; }
        static constexpr VxSize           GetBinding() { return type; }
        static constexpr VxSize           GetOffset() { return Offset; }
    };

    namespace VertexShaderInputUtils
    {
        // https://stackoverflow.com/a/34672753/12576629
        template <template <VxDataBufferType type, VxSize Binding, VxSize Offset> class base, typename derived>
        struct is_base_of_template_impl
        {
            template <VxDataBufferType type, VxSize Binding, VxSize Offset>
            static constexpr std::true_type  test(const base<type, Binding, Offset>*);
            static constexpr std::false_type test(...);
            using type = decltype(test(std::declval<derived*>()));
        };

        template <template <VxDataBufferType, VxSize, VxSize> class base, typename derived>
        using is_base_of_template = typename is_base_of_template_impl<base, derived>::type;

        template <typename T> inline constexpr bool IsBuffer = is_base_of_template<VxDataBuffer, T>::value;

        template <typename> constexpr VxSize                                     CountBuffers() { return 0; }
        template <typename T, typename First, typename... Rest> constexpr VxSize CountBuffers()
        {
            return (IsBuffer<First> ? 1 : 0) + CountBuffers<T, Rest...>();
        }

        template <int I, int J, int VertexBuffers, int VertexInputFieldsCount>
        constexpr void GenerateArrays(std::array<VkVertexInputBindingDescription, VertexBuffers>& bindings,
            std::array<VkVertexInputAttributeDescription, VertexInputFieldsCount>& attributes, int binding)
        {
        }
        template <int I, int J, int VertexBuffers, int VertexInputFieldsCount, typename First, typename... Rest>
        constexpr void GenerateArrays(std::array<VkVertexInputBindingDescription, VertexBuffers>& bindings,
            std::array<VkVertexInputAttributeDescription, VertexInputFieldsCount>& attributes, int binding)
        {
            if constexpr (IsBuffer<First>)
            {
                bindings[I] = First::GetBindingDescription(sizeof(First));
                GenerateArrays<I + 1, J, VertexBuffers, VertexInputFieldsCount, Rest...>(
                    bindings, attributes, bindings[I].binding);
            }
            if constexpr (IsVertexField<First>)
            {
                VX_CORE_STATIC_ASSERT(I != 0, "found a field before any subclasses of VxDataBuffer");
                attributes[J] = First::GetAttributeDescription(binding);
                GenerateArrays<I, J + 1, VertexBuffers, VertexInputFieldsCount, Rest...>(bindings, attributes, binding);
            }
        }

        template <VxSize I, VxSize Size, VxSize Offset>
        constexpr void GenerateVAOLayout(std::array<VulkanVertexArrayLayoutElement, Size>& arr)
        {
        }

        template <VxSize I, VxSize Size, VxSize Offset, typename First, typename... Rest>
        constexpr void GenerateVAOLayout(std::array<VulkanVertexArrayLayoutElement, Size>& arr)
        {
            if constexpr (IsBuffer<First>)
            {
                arr[I] = { Offset + First::GetOffset(), sizeof(First) };
                GenerateVAOLayout<I + 1, Size, Offset + First::GetOffset() + sizeof(First), Rest...>(arr);
            }
            else
            {
                GenerateVAOLayout<I, Size, Offset, Rest...>(arr);
            }
        }
    }

    template <typename T>
    concept VulkanVertexShaderInputsParam
        = VertexShaderInputUtils::IsBuffer<T> || VertexShaderInputUtils::IsVertexField<T>;

    template <VulkanVertexShaderInputsParam... Args> struct VulkanVertexShaderInputs
    {
    private:
        static constexpr VxSize s_VertexBuffers = VertexShaderInputUtils::CountBuffers<std::nullptr_t, Args...>();
        static constexpr VxSize s_VertexFieldsCount = sizeof...(Args) - s_VertexBuffers;

    public:
        static constexpr auto GetAttributeDescriptions()
        {
            std::array<VkVertexInputBindingDescription, s_VertexBuffers>       binding_descriptions {};
            std::array<VkVertexInputAttributeDescription, s_VertexFieldsCount> attribute_descriptions {};
            VertexShaderInputUtils::GenerateArrays<0, 0, s_VertexBuffers, s_VertexFieldsCount, Args...>(
                binding_descriptions, attribute_descriptions, -1);
            return std::make_tuple(binding_descriptions, attribute_descriptions);
        }
        template <VxSize BaseOffset> static constexpr auto GetVertexArrayLayout()
        {
            std::array<VulkanVertexArrayLayoutElement, s_VertexBuffers> arr;
            VertexShaderInputUtils::GenerateVAOLayout<0, s_VertexBuffers, BaseOffset, Args...>(arr);
            VulkanVertexArrayLayout<s_VertexBuffers> layout(arr);
            return layout;
        }
    };
}