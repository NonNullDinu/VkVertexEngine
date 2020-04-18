#pragma once

#ifdef _WIN32
    #ifdef VX_BUILD_SHARED
        #define VX_PUBLIC_API __declspec(dllexport)
    #else
        #define VX_PUBLIC_API __declspec(dllimport)
    #endif
#else 
    #define VERTEX_PUBLIC_API
#endif