#pragma once
#include <variant>
namespace Vertex {
    typedef std::variant<float, double, glm::vec2, glm::vec3, glm::vec4, glm::mat4> OpenGLUniformValue;
    class OpenGLUniform
    {
    private:
        int m_Location;
        std::string m_UniformName;

        template<typename T>
        void operator()(T value);
    public:
        OpenGLUniform(int location, std::string uniform_name);
        OpenGLUniform(const OpenGLUniform& uniform);
        friend bool operator<(OpenGLUniform, OpenGLUniform);
        std::string GetName();

        void operator=(OpenGLUniformValue value);
    };
}