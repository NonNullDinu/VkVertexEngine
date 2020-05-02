#include "VertexArray.h"

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