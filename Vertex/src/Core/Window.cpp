#include "Window.h"

#if defined(_WIN32)
#include "Platforms/Windows/WindowsWindow.h"
#elif defined(__linux__)
#include "Platforms/Linux/LinuxWindow.h"
#endif

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