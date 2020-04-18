#pragma once

#ifdef _WIN32
    #define VERTEX_PUBLIC_API __declspec(dllexport)
#else 
    #define VERTEX_PUBLIC_API
#endif