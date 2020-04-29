#pragma once
namespace Vertex {
    class OpenGLUniform {
    private:
        int m_Location;
        std::string m_UniformName;
    public:
        OpenGLUniform(int location, std::string uniform_name);
        OpenGLUniform(const OpenGLUniform& uniform);
        friend bool operator<(OpenGLUniform, OpenGLUniform);
        std::string GetName();

        template<typename T>
        void operator()(T value);
    };
}