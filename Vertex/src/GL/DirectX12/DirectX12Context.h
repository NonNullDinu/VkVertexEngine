#pragma once

// -----------------------------

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#if defined(min) // weird macros we already have in <algorithm>
    #undef min
#endif

#if defined(max) // weird macros we already have in <algorithm>
    #undef max
#endif

#define GLFW_EXPOSE_NATIVE_WIN32

#include <GLFW/glfw3native.h>

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using namespace Microsoft::WRL;

// -----------------------------

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
 
// D3D12 extension library.
#include <d3dx12.h>

#include "GL/GraphicsContext.h"

namespace Vertex {

    class DirectX12Context : public GraphicsContext
    {
    public:
        DirectX12Context(GLFWwindow* window);

        void Render() override {}

        void SwapBuffers() override {}

        void NotifyResize(int new_width, int new_height) override {}

        void CleanUpContext() override {}

    private:
        HWND m_WindowHandle;

        static const uint16_t m_NumBackBuffers = 3;

        /*
        The m_UseWarp variable controls whether to use a software rasterizer
        (Windows Advanced Rasterization Platform - WARP) or not.
        The software rasterizer allows the graphics programmer to
        access the full set of advanced rendering features that
        may not be available in the hardware (for example, when running on older GPUs).
        The WARP device can also be used to verify the results of a rendering technique
        if the quality of the vendor supplied display driver is in question.
        */
        bool m_UseWARP;

        RECT m_WindowRect;
 
        // DirectX 12 Objects
        Microsoft::WRL::ComPtr<ID3D12Device2> m_Device;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
        Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_BackBuffers[m_NumBackBuffers];
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocators[m_NumBackBuffers];
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;

        uint32_t m_RTVDescriptorSize;
        uint16_t m_CurrentBackBufferIndex;

        int m_CurrentWidth, m_CurrentHeight;

        bool m_NeedViewportUpdate;
    };

}
