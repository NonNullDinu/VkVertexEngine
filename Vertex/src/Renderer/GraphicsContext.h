#pragma once

#include "Core/Core.h"

namespace Vertex {

    class GraphicsContext
    {
    public:
        virtual void SwapBuffers() = 0;

        virtual void NotifyResize(int new_width, int new_height) = 0;

        virtual void CleanUpContext() = 0;
    };

}