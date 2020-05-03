#include "DirectX12Context.h"

namespace Vertex {

    static bool s_DirectXInitialized = false;

    DirectX12Context::DirectX12Context(GLFWwindow* window)
        : m_WindowHandle(glfwGetWin32Window(window)),
          m_UseWARP(false)
    {
        
    }

}