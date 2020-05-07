#include "GraphicsAPI.h"

#include "OpenGL/OpenGLGraphicsAPI.h"

namespace Vertex {

    GraphicsAPI* GraphicsAPI::Create()
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLGraphicsAPI();
#else
        return nullptr; // for now
#endif
    }

}