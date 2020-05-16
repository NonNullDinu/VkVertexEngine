#pragma once

#include "Uniform.h"

namespace Vertex
{
    class UniformPack
    {
    public:
        const Uniform& operator[](std::string name)
        {
            decltype(m_Uniforms)::const_iterator it = m_Uniforms.find(name);
            if (it != m_Uniforms.end())
            {
                return *it->second;
            }
            Uniform* new_uniform = nullptr//new Uniform(name)
                ;
            m_Uniforms.insert({ name, new_uniform });
            return *new_uniform;
        }

    private:
        std::map<std::string, Uniform*> m_Uniforms;
    };
}