#include "OpenGLUniformPack.h"
namespace Vertex {
    OpenGLUniformPack::OpenGLUniformPack(uint32_t * program_id)
        : m_ProgramId(*program_id)
    {
    }

    void OpenGLUniformPack::LoadUniformLocation(std::string uniform_var_name)
    {
        OpenGLUniform uniform(glGetUniformLocation(m_ProgramId, uniform_var_name.c_str()), uniform_var_name);
        m_UniformMap.insert(std::pair<std::string, OpenGLUniform>(uniform_var_name, uniform));
    }

    void OpenGLUniformPack::StartLoadingLocations()
    {
        // empty for now
    }

    void OpenGLUniformPack::EndLoadingLocations()
    {
        // empty for now
    }
    OpenGLUniform& OpenGLUniformPack::operator[](std::string uniform_name)
    {
        return m_UniformMap.at(uniform_name);
    }
}
