#include "Input.h"

namespace Vertex {

    Input* Input::s_Instance = nullptr;

    void Input::Init()
    {
#if defined(_WIN32)
        s_Instance = new WindowsInput();
#elif defined(__linux__)
        s_Instance = new LinuxInput();
#else
        #error Unsupported platform
#endif
    }

}