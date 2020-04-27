#pragma once

#include "Core/Core.h"

namespace Vertex {

    class GraphicsContext
    {
    public:
        virtual void SwapBuffers() = 0;
    };

}