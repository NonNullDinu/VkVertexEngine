#pragma once

#if defined(_WIN32)
    #if defined(VX_BUILD_SHARED)
        #define VX_PUBLIC_API __declspec(dllexport)
    #else
        #define VX_PUBLIC_API __declspec(dllimport)
    #endif // VX_BUILD_SHARED
#else
    #define VERTEX_PUBLIC_API
#endif // _WIN32