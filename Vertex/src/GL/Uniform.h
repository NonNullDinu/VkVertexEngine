#pragma once

#include "ShaderDataType.h"

namespace Vertex
{
    class Uniform
    {
    public:
        Uniform(std::string name) : m_Name(name) { }

        virtual void Load(ShaderDataType value) = 0;
        inline void  operator=(ShaderDataType value) { Load(value); }

    private:
        std::string m_Name;
    };
}
