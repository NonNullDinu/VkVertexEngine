#include "Input.h"
#if defined(_WIN32)
    #include "Platforms/Windows/WindowsInput.h"
#elif defined(__linux__)
    #include "Platforms/Linux/LinuxInput.h"
#endif

namespace Vertex
{

#if defined(_WIN32)
    Input* Input::s_Instance = new WindowsInput();
#elif defined(__linux__)
    Input* Input::s_Instance = new LinuxInput();
#else
    #error Unsupported platform
#endif

}