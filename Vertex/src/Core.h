#pragma once

#if defined(_WIN32)
    #define VX_PUBLIC_API __declspec(dllexport)

#elif defined(__linux__) || defined(__APPLE__)
    #define VERTEX_PUBLIC_API

#else
    #error "Unknown platform. Vertex only supports windows, mac, and linux."

#endif // _WIN32