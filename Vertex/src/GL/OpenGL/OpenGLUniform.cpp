#include "OpenGLUniform.h"

namespace Vertex {

    OpenGLUniform::OpenGLUniform(int location, std::string uniform_name)
        : m_Location(location), m_UniformName(uniform_name)
    {
    }

    OpenGLUniform::OpenGLUniform(const OpenGLUniform& uniform)
        : OpenGLUniform(uniform.m_Location, uniform.m_UniformName)
    {
    }

    bool operator<(OpenGLUniform a, OpenGLUniform b)
    {
        return a.m_UniformName < b.m_UniformName;
    }

    std::string OpenGLUniform::GetName()
    {
        return m_UniformName;
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
    
}