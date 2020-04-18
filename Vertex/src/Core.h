#pragma once

#if defined(_WIN32)
#   if defined(VX_BUILD_SHARED)
#       define VX_PUBLIC_API __declspec(dllexport)
#   else
#       define VX_PUBLIC_API __declspec(dllimport)
#   endif // VX_BUILD_SHARED
#elif defined(__linux__) || defined(__APPLE__)
#   define VERTEX_PUBLIC_API
#else
#   error "Unknown platform. Vertex only supports windows, mac, and linux."
#endif // _WIN32