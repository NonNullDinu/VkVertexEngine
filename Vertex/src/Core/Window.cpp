#include "Window.h"

namespace Vertex
{

    Window* Window::Create()
    {
#if defined(_WIN32)
        return new WindowsWindow();
#elif defined(__linux__)
        return new LinuxWindow();
#else
        #error Unsupported platform
#endif
    }

}