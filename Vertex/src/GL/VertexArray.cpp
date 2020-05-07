#include "VertexArray.h"

#if defined(VX_RENDER_API_OPENGL)
    #include "OpenGL/OpenGLVertexArray.h"
#endif
// ... per rendering api

namespace Vertex {

    VertexArray* VertexArray::Create()
    {
#if defined(VX_RENDER_API_OPENGL)
        return new OpenGLVertexArray();
#else
        return nullptr;
#endif
    }

}