#pragma once

#include "GL/GraphicsAPI.h"

namespace Vertex
{

    class OpenGLGraphicsAPI : public GraphicsAPI
    {
    public:
        inline void Clear() override
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // for now
        }

        inline void Clear(const glm::vec3& color) override
        {
            glClearColor(color.r, color.g, color.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        inline void Clear(const glm::vec4& color) override
        {
            glClearColor(color.r, color.g, color.b, color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        inline void DrawIndexed(std::shared_ptr<VertexArray> vertex_array) override
        {
            glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr); // for now
        }
    };

}