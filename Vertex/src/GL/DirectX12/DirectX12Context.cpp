#include "DirectX12Context.h"

namespace Vertex {

    static bool s_DirectX12Initialized = false;

    static void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
            throw std::exception();
    }

    DirectX12Context::DirectX12Context(GLFWwindow* window)
        : m_WindowHandle(glfwGetWin32Window(window)),
          m_UseWARP(false)
    {
        CoreLogger::Debug("Initializing DirectX...");

        EnableDebugLayer();

        Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetAdapter();

        CoreLogger::Debug("Got adapter");

        m_Device = CreateDevice(dxgiAdapter4);

        CoreLogger::Debug("Created DirectX device");

        m_CommandQueue = CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT);

        CoreLogger::Debug("Created command queue");

        m_SwapChain = CreateSwapChain(m_CurrentWidth, m_CurrentHeight, VX_NUM_BACK_BUFFERS);

        CoreLogger::Debug("Created swap chain");

        m_CurrentBackBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();

        m_RTVDescriptorHeap = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, VX_NUM_BACK_BUFFERS);
        m_RTVDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        UpdateRenderTargetViews();

        for (int i = 0; i < VX_NUM_BACK_BUFFERS; ++i)
            m_CommandAllocators[i] = CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT);

        m_CommandList = CreateCommandList(m_CommandAllocators[m_CurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);

        m_Fence = CreateFence();
        m_FenceEvent = CreateEventHandle();

        s_DirectX12Initialized = true;

        CoreLogger::Debug("Initialized DirectX");
    }

    DirectX12Context::~DirectX12Context()
    {
        Flush(m_FenceValue);
        ::CloseHandle(m_FenceEvent);
    }

    void DirectX12Context::NotifyResize(int new_width, int new_height)
    {
        m_CurrentWidth = new_width;
        m_CurrentHeight = new_height;
        m_NeedViewportUpdate = true;
    }

    void DirectX12Context::Render()
    {
        auto commandAllocator = m_CommandAllocators[m_CurrentBackBufferIndex];
        auto backBuffer = m_BackBuffers[m_CurrentBackBufferIndex];
    
        commandAllocator->Reset();
        m_CommandList->Reset(commandAllocator.Get(), nullptr);

        // Clear the render target.
        {
            CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
                backBuffer.Get(),
                D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
            
            m_CommandList->ResourceBarrier(1, &barrier);

            FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };

            CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
                m_CurrentBackBufferIndex, m_RTVDescriptorSize);
        
            m_CommandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
        }

        // Present
        {
            CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
                backBuffer.Get(),
                D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

            m_CommandList->ResourceBarrier(1, &barrier);

            ThrowIfFailed(m_CommandList->Close());
 
            ID3D12CommandList* const commandLists[] = {
                m_CommandList.Get()
            };

            m_CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

            uint32_t syncInterval = m_VSync ? 1 : 0;
            uint32_t presentFlags = m_TearingSupported && !m_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;

            ThrowIfFailed(m_SwapChain->Present(syncInterval, presentFlags));
            
            m_FrameFenceValues[m_CurrentBackBufferIndex] = Signal(m_FenceValue);

            m_CurrentBackBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();
 
            WaitForFenceValue(m_FrameFenceValues[m_CurrentBackBufferIndex]);
        }
    }

    // Initialize DirectX12

    void DirectX12Context::EnableDebugLayer()
    {
#if defined(VX_CONFIGURATION_DEBUG)
        Microsoft::WRL::ComPtr<ID3D12Debug> debug_interface;
        ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_interface)));
        debug_interface->EnableDebugLayer();
#endif
    }

    bool DirectX12Context::CheckTearingSupport()
    {
        int allowTearing = FALSE;
    
        // Rather than create the DXGI 1.5 factory interface directly, we create the
        // DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
        // graphics debugging tools which will not support the 1.5 factory interface 
        // until a future update.
        ComPtr<IDXGIFactory4> factory4;
        if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
        {
            ComPtr<IDXGIFactory5> factory5;
            if (SUCCEEDED(factory4.As(&factory5)))
            {
                if (FAILED(factory5->CheckFeatureSupport(
                    DXGI_FEATURE_PRESENT_ALLOW_TEARING, 
                    &allowTearing, sizeof(allowTearing))))
                {
                    allowTearing = FALSE;
                }
            }
        }
    
        return allowTearing == TRUE;
    }

    Microsoft::WRL::ComPtr<IDXGIAdapter4> DirectX12Context::GetAdapter()
    {
        Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory;

        uint32_t createFactoryFlags = 0;

#if defined(VX_CONFIGURATION_DEBUG)
        createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
 
        ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));

        Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter1;
        Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgiAdapter4;
        
        if (m_UseWARP)
        {
            ThrowIfFailed(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter1)));
            ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
        }
        else
        {
            SIZE_T maxDedicatedVideoMemory = 0;
            for (uint32_t i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
            {
                DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
                dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);
    
                // Check to see if the adapter can create a D3D12 device without actually 
                // creating it. The adapter with the largest dedicated video memory
                // is favored.
                if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
                    SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(), 
                        D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) && 
                    dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory )
                {
                    maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
                    ThrowIfFailed(dxgiAdapter1.As(&dxgiAdapter4));
                }
            }
        }
    
        return dxgiAdapter4;
    }

    Microsoft::WRL::ComPtr<ID3D12Device2> DirectX12Context::CreateDevice(Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter)
    {
        CoreLogger::Debug("Creating device...");

        Microsoft::WRL::ComPtr<ID3D12Device2> d3d12Device2;
        ThrowIfFailed(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device2)));

        CoreLogger::Debug("Created device");

#if defined(VX_CONFIGURATION_DEBUG)

        Microsoft::WRL::ComPtr<ID3D12InfoQueue> pInfoQueue;
        if (SUCCEEDED(d3d12Device2.As(&pInfoQueue)))
        {
            pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
            pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
            pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

            // // Suppress whole categories of messages
            // D3D12_MESSAGE_CATEGORY Categories[] = {};
    
            // Suppress messages based on their severity level
            D3D12_MESSAGE_SEVERITY Severities[] = {
                D3D12_MESSAGE_SEVERITY_INFO
            };
    
            // Suppress individual messages by their ID
            D3D12_MESSAGE_ID DenyIds[] = {
                D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,   // I'm really not sure how to avoid this message.
                D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,                         // This warning occurs when using capture frame while graphics debugging.
                D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,                       // This warning occurs when using capture frame while graphics debugging.
            };
    
            D3D12_INFO_QUEUE_FILTER NewFilter = {};
            //NewFilter.DenyList.NumCategories = _countof(Categories);
            //NewFilter.DenyList.pCategoryList = Categories;
            NewFilter.DenyList.NumSeverities = _countof(Severities);
            NewFilter.DenyList.pSeverityList = Severities;
            NewFilter.DenyList.NumIDs = _countof(DenyIds);
            NewFilter.DenyList.pIDList = DenyIds;
    
            ThrowIfFailed(pInfoQueue->PushStorageFilter(&NewFilter));
        }
#endif // VX_CONFIGURATION_DEBUG
    
        return d3d12Device2;
    }

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> DirectX12Context::CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type)
    {
        ComPtr<ID3D12CommandQueue> d3d12CommandQueue;
 
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = type;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;
    
        ThrowIfFailed(m_Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&d3d12CommandQueue)));
    
        return d3d12CommandQueue;
    }

    Microsoft::WRL::ComPtr<IDXGISwapChain4> DirectX12Context::CreateSwapChain(uint32_t width, uint32_t height, uint32_t bufferCount)
    {
        Microsoft::WRL::ComPtr<IDXGISwapChain4> dxgiSwapChain4;
        Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory4;
        uint32_t createFactoryFlags = 0;

#if defined(VX_CONFIGURATION_DEBUG)
        createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
 
        ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = m_CurrentWidth;
        swapChainDesc.Height = m_CurrentHeight;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.Stereo = FALSE;
        swapChainDesc.SampleDesc = { 1, 0 };
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = bufferCount;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        // It is recommended to always allow tearing if tearing support is available.
        swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain1;
        ThrowIfFailed(dxgiFactory4->CreateSwapChainForHwnd(
            m_CommandQueue.Get(),
            m_WindowHandle,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain1));
    
        // Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
        // will be handled manually.
        ThrowIfFailed(dxgiFactory4->MakeWindowAssociation(m_WindowHandle, DXGI_MWA_NO_ALT_ENTER));
    
        ThrowIfFailed(swapChain1.As(&dxgiSwapChain4));
    
        return dxgiSwapChain4;
    }

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DirectX12Context::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors)
    {
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;
    
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = numDescriptors;
        desc.Type = type;
    
        ThrowIfFailed(m_Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));
    
        return descriptorHeap;
    }

    void DirectX12Context::UpdateRenderTargetViews()
    {
        m_RTVDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    
        for (int i = 0; i < VX_NUM_BACK_BUFFERS; ++i)
        {
            ComPtr<ID3D12Resource> backBuffer;
            ThrowIfFailed(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));
    
            m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);
    
            m_BackBuffers[i] = backBuffer;
    
            rtvHandle.Offset(m_RTVDescriptorSize);
        }
    }

    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> DirectX12Context::CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type)
    {
        ComPtr<ID3D12CommandAllocator> commandAllocator;
        ThrowIfFailed(m_Device->CreateCommandAllocator(type, IID_PPV_ARGS(&commandAllocator)));
    
        return commandAllocator;
    }

    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> DirectX12Context::CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type)
    {
        ComPtr<ID3D12GraphicsCommandList> commandList;
        ThrowIfFailed(m_Device->CreateCommandList(0, type, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));
        
        ThrowIfFailed(commandList->Close());
    
        return commandList;
    }

    Microsoft::WRL::ComPtr<ID3D12Fence> DirectX12Context::CreateFence()
    {
        ComPtr<ID3D12Fence> fence;
    
        ThrowIfFailed(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
    
        return fence;
    }

    HANDLE DirectX12Context::CreateEventHandle()
    {
        HANDLE fenceEvent;
        
        fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
        assert(fenceEvent && "Failed to create fence event.");
    
        return fenceEvent;
    }

    uint64_t DirectX12Context::Signal(uint64_t& fenceValue)
    {
        uint64_t fenceValueForSignal = ++fenceValue;
        ThrowIfFailed(m_CommandQueue->Signal(m_Fence.Get(), fenceValueForSignal));
    
        return fenceValueForSignal;
    }

    void DirectX12Context::WaitForFenceValue(uint64_t fenceValue,
        std::chrono::milliseconds duration /* = std::chrono::milliseconds::max() */)
    {
        if (m_Fence->GetCompletedValue() < fenceValue)
        {
            ThrowIfFailed(m_Fence->SetEventOnCompletion(fenceValue, m_FenceEvent));
            ::WaitForSingleObject(m_FenceEvent, static_cast<DWORD>(duration.count()));
        }
    }

    void DirectX12Context::Flush(uint64_t& fenceValue)
    {
        uint64_t fenceValueForSignal = Signal(fenceValue);
        WaitForFenceValue(fenceValueForSignal);
    }

}