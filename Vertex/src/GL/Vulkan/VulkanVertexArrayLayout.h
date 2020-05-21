#pragma once

namespace Vertex
{
    struct VulkanVertexArrayLayoutElement
    {
        VxSize vertex_offset;
        VxSize internal_size;

        constexpr VulkanVertexArrayLayoutElement() : vertex_offset(0), internal_size(0) { }
        constexpr VulkanVertexArrayLayoutElement(VxSize p_vertex_offset, VxSize p_internal_size)
            : vertex_offset(p_vertex_offset), internal_size(p_internal_size)
        {
        }
    };
    template <VxSize Size> struct VulkanVertexArrayLayout
    {
    private:
        std::array<VulkanVertexArrayLayoutElement, Size> m_Elements;

    public:
        constexpr VulkanVertexArrayLayout(decltype(m_Elements) p_elements) : m_Elements(p_elements) { }
        typename decltype(m_Elements)::iterator begin() { return m_Elements.begin(); }
        typename decltype(m_Elements)::iterator end() { return m_Elements.end(); }
    };
}