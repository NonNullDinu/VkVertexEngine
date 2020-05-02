#include "GraphicsContext.h"

namespace Vertex {

    GraphicsContext* GraphicsContext::Create(GLWwindow* window_handle)
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLContext(window_handle);
#endif
    }

}