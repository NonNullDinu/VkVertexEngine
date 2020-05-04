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

        void Render() override;

        void SwapBuffers() override {}

        void NotifyResize(int new_width, int new_height) override;

        void CleanUpContext() override {}

    private:
        HWND m_WindowHandle;

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

        /*
        Store previous window dimensions if the screen is maximized
        */
        RECT m_WindowRect;
 
        // DirectX 12 Objects
        Microsoft::WRL::ComPtr<ID3D12Device2> m_Device;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CommandQueue;
        Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_BackBuffers[VX_NUM_BACK_BUFFERS];
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_CommandList;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocators[VX_NUM_BACK_BUFFERS];
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RTVDescriptorHeap;

        uint32_t m_RTVDescriptorSize; // Render Target View
        uint16_t m_CurrentBackBufferIndex;

        // Synchronization objects
        Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
        uint64_t m_FenceValue = 0;
        uint64_t m_FrameFenceValues[VX_NUM_BACK_BUFFERS] = {};
        HANDLE m_FenceEvent;

        bool m_VSync = true;
        bool m_TearingSupported = false;
        bool m_Fullscreen = false; // tracks the fullscreen state of the window

        int m_CurrentWidth, m_CurrentHeight;

        bool m_NeedViewportUpdate;

    private:
        void EnableDebugLayer();

        static void CheckTearingSupport(); // for variable refresh rates

        Microsoft::WRL::ComPtr<IDXGIAdapter4> GetAdapter();

        Microsoft::WRL::ComPtr<ID3D12Device2> CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter);

        Microsoft::WRL::ComPtr<ID3D12CommandQueue> CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type);

        Microsoft::WRL::ComPtr<IDXGISwapChain4> CreateSwapChain(uint32_t width, uint32_t height, uint32_t bufferCount);

        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors);

        void UpdateRenderTargetViews();

        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type);

        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type);

        Microsoft::WRL::ComPtr<ID3D12Fence> CreateFence();

        HANDLE CreateEventHandle();

        uint64_t Signal();

        void WaitForFenceValue(uint64_t frameFenceValue, std::chrono::milliseconds duration = std::chrono::milliseconds::max());

        void Flush();
    };

}
