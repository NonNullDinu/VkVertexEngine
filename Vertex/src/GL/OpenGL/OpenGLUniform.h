#pragma once

#include <variant>

namespace Vertex {
    typedef std::variant<float, double, glm::vec2, glm::vec3, glm::vec4, glm::mat4> OpenGLUniformValue;

    class OpenGLUniform
    {
    public:
        OpenGLUniform(int location, const char* uniform_name);
        
        friend bool operator<(OpenGLUniform, OpenGLUniform);
        
        inline const char* GetName() const { return m_UniformName; }

        void operator=(OpenGLUniformValue value);

    private:
        int m_Location;
        const char* m_UniformName;

        template<typename T>
        void operator()(T value);
    };

    // ----------------------------------
    // ---------- Uniform Pack ----------
    // ----------------------------------

    class OpenGLUniformPack
    {
    public:
        OpenGLUniformPack(uint32_t* program_id);

        void LoadUniformLocation(const char* uniform_var_name);

        OpenGLUniform& operator[](const char* uniform_name);

    private:
        std::map<const char*, OpenGLUniform> m_UniformMap;
        const uint32_t& m_ProgramId;
    };

}