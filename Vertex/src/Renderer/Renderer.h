#pragma once

#include "Core/Core.h"

#include "GL/GraphicsAPI.h"

#include "GL/VertexArray.h"

#include <glm/glm.hpp>

namespace Vertex {

    enum class RenderAPI
    {
        OpenGL, Vulkan, DirectX12
    };

    class Renderer
    {
    public:
        static void Clear()
        {
            s_GraphicsAPI->Clear();
        }

        static void Clear(const glm::vec3& color)
        {
            s_GraphicsAPI->Clear(color);
        }

        static void Clear(const glm::vec4& color)
        {
            s_GraphicsAPI->Clear(color);
        }

        static void BeginScene();
        static void EndScene();

        static void Submit(std::shared_ptr<VertexArray> vertex_array);

        static RenderAPI GetAPI();

    private:
        static GraphicsAPI* s_GraphicsAPI;
    };

}
