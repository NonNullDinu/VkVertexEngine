#pragma once

namespace Vertex {

    enum class RenderAPI
    {
        OpenGL, Vulkan
    };

#define DEFAULT_RENDER_API RenderAPI::OpenGL

    class Renderer
    {
    public:
        static inline RenderAPI GetRenderAPI()              { return s_RenderAPI; }
        static inline void      SetRenderAPI(RenderAPI api) { s_RenderAPI = api; }

    private:
        static RenderAPI s_RenderAPI;
    };

}