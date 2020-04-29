#include "OpenGLShader.h"

namespace Vertex {

    OpenGLShader::OpenGLShader(const char* vertex_src, const char* fragment_src) : m_UniformPack (&m_ID)
    {
        // from khronos.org

        // Create an empty vertex shader handle
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        glShaderSource(vertex_shader, 1, &vertex_src, 0);

        // Compile the vertex shader
        glCompileShader(vertex_shader);

        GLint compile_success = 0;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_success);

        if(compile_success == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertex_shader, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the shader anymore.
            glDeleteShader(vertex_shader);
            
            Logger::GetCoreLogger()->error("{0}", infoLog.data());
            VX_CORE_ASSERT(false, "Vertex Shader compilation failed");
        }

        // Create an empty fragment shader handle
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        glShaderSource(fragment_shader, 1, &fragment_src, 0);

        // Compile the fragment shader
        glCompileShader(fragment_shader);

        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_success);

        if (compile_success == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragment_shader, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the shader anymore.
            glDeleteShader(fragment_shader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertex_shader);
            
            Logger::GetCoreLogger()->error("{0}", infoLog.data());
            VX_CORE_ASSERT(false, "Fragment Shader compilation failed");
        }

        // Vertex and fragment shaders are successfully compiled.
        // Link them together into a program and get a program object.
        m_ID = glCreateProgram();

        // Attach our shaders to our program
        glAttachShader(m_ID, vertex_shader);
        glAttachShader(m_ID, fragment_shader);

        // Link our program
        glLinkProgram(m_ID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&isLinked);

        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_ID, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the program anymore.
            glDeleteProgram(m_ID);
            // Don't leak shaders either.
            glDeleteShader(vertex_shader);
            glDeleteShader(fragment_shader);

            Logger::GetCoreLogger()->error("{0}", infoLog.data());
            VX_CORE_ASSERT(false, "Shader linking failed");
        }

        // Always detach shaders after a successful link.
        glDetachShader(m_ID, vertex_shader);
        glDetachShader(m_ID, fragment_shader);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_ID);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_ID);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::StartLoadingUniformsToPack()
    {
        m_UniformPack.StartLoadingLocations();
    }

    void OpenGLShader::LoadToUniformToPack(std::string uniform_var_name)
    {
        m_UniformPack.LoadUniformLocation(uniform_var_name);
    }

    void OpenGLShader::StopLoadingUniformsToPack()
    {
        m_UniformPack.StartLoadingLocations();
    }

    OpenGLUniform& OpenGLShader::operator[](std::string name)
    {
        return m_UniformPack[name];
    }

}