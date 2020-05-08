#include "GraphicsAPI.h"

#if defined(VX_RENDER_API_OPENGL)
    #include "OpenGL/OpenGLGraphicsAPI.h"
#endif

namespace Vertex
{

    GraphicsAPI* GraphicsAPI::Create()
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLGraphicsAPI();
#else
        return nullptr; // for now
#endif
    }

}