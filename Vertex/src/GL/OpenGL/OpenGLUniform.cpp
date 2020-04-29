#include "OpenGLUniform.h"

namespace Vertex {

    OpenGLUniform::OpenGLUniform(int location, const char* uniform_name)
        : m_Location(location), m_UniformName(uniform_name)
    {
    }

    bool operator<(OpenGLUniform a, OpenGLUniform b)
    {
        return a.m_UniformName < b.m_UniformName;
    }

    template<>
    void OpenGLUniform::operator()(float value)
    {
        glUniform1f(m_Location, value);
    }

    template<>
    void OpenGLUniform::operator()(double value)
    {
        glUniform1d(m_Location, value);
    }

    template<>
    void OpenGLUniform::operator()(glm::vec2 value)
    {
        glUniform2f(m_Location, value.x, value.y);
    }

    template<>
    void OpenGLUniform::operator()(glm::vec3 value)
    {
        glUniform3f(m_Location, value.x, value.y, value.z);
    }

    template<>
    void OpenGLUniform::operator()(glm::vec4 value)
    {
        glUniform4f(m_Location, value.x, value.y, value.z, value.w);
    }

    template<>
    void OpenGLUniform::operator()(glm::mat4 value)
    {
        glUniformMatrix4fv(m_Location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLUniform::operator=(OpenGLUniformValue value)
    {
        std::visit([this](auto&& v) -> void
        {
            this->operator()(v);
        }, value);
    }

    // ----------------------------------
    // ---------- Uniform Pack ----------
    // ----------------------------------

    OpenGLUniformPack::OpenGLUniformPack(uint32_t * program_id)
        : m_ProgramId(*program_id)
    {
    }

    void OpenGLUniformPack::LoadUniformLocation(const char* uniform_var_name)
    {
        OpenGLUniform uniform(glGetUniformLocation(m_ProgramId, uniform_var_name), uniform_var_name);
        m_UniformMap.insert(std::pair<const char*, OpenGLUniform>(uniform_var_name, uniform));
    }
    
    OpenGLUniform& OpenGLUniformPack::operator[](const char* uniform_name)
    {
        return m_UniformMap.at(uniform_name);
    }
    
}