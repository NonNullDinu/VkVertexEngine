#pragma once

#include "OpenGLUniform.h"
#include <map>

namespace Vertex {
    class OpenGLUniformPack
    {
    private:
        std::map<std::string, OpenGLUniform> m_UniformMap;
        const uint32_t& m_ProgramId;

    public:
        OpenGLUniformPack(uint32_t* program_id);

        void StartLoadingLocations();
        void LoadUniformLocation(std::string uniform_var_name);
        void EndLoadingLocations();

        OpenGLUniform& operator[](std::string uniform_name);
    };
}