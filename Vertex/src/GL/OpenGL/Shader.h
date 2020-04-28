#pragma once

#include "Core/Core.h"

namespace Vertex {

    class Shader
    {
    public:
        Shader(const char* vertex_src, const char* fragment_src);
        ~Shader();

        void Bind() const;
        void Unbind() const;

    private:
        uint32_t m_ID;
    };

}