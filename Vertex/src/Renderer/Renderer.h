#pragma once

#include "Core/Core.h"

#include "GL/GraphicsAPI.h"

#include "GL/VertexArray.h"

#include "GL/Shader.h"

#include "Camera.h"

namespace Vertex
{

    enum class RenderAPI
    {
        OpenGL, Vulkan, DirectX12
    };

    struct Scene
    {
        Camera& camera;

        Scene(Camera& _camera)
            : camera(_camera)
        {
        }
    };

    class VX_PUBLIC_API Renderer
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

        static void BeginScene(Camera& camera);
        static void EndScene();

        static void Submit(std::shared_ptr<VertexArray>& vertex_array, const std::shared_ptr<Shader>& shader);

        static RenderAPI GetAPI();

    private:
        static GraphicsAPI* s_GraphicsAPI;
        static Scene* s_Scene;
    };

}
