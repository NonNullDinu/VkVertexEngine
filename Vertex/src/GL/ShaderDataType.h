#pragma once
namespace Vertex
{
    typedef std::variant<
        // scalars
        float, double, int, std::int64_t, bool, // all the scalars

        // vectors
        glm::vec1, glm::vec2, glm::vec3, glm::vec4,             // float vectors
        glm::dvec1, glm::dvec2, glm::dvec3, glm::dvec4,         // double vectors
        glm::ivec1, glm::ivec2, glm::ivec3, glm::ivec4,         // 32bit int vectors
        glm::i64vec1, glm::i64vec2, glm::i64vec3, glm::i64vec4, // 64bit int vectors
        glm::bvec1, glm::bvec2, glm::bvec3, glm::bvec4,         // boolean vectors

        // square matrices
        glm::mat2, glm::mat3, glm::mat4,          // square float matrices
        glm::dmat2, glm::dmat3, glm::dmat4,       // square double matrices
        glm::imat2, glm::imat3, glm::imat4,       // square 32bit int matrices
        glm::i64mat2, glm::i64mat3, glm::i64mat4, // square 64bit int matrices

        // non-square matrices
        glm::mat2x3, glm::mat2x4, glm::mat3x2, glm::mat3x4, glm::mat4x2, glm::mat4x3,                  // float
        glm::dmat2x3, glm::dmat2x4, glm::dmat3x2, glm::dmat3x4, glm::dmat4x2, glm::dmat4x3,            // double
        glm::imat2x3, glm::imat2x4, glm::imat3x2, glm::imat3x4, glm::imat4x2, glm::imat4x3,            // 32bit int
        glm::i64mat2x3, glm::i64mat2x4, glm::i64mat3x2, glm::i64mat3x4, glm::i64mat4x2, glm::i64mat4x3 // 64bit int

        // for now these should be enough

        >
        ShaderDataType;
}