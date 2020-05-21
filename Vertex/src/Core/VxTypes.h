#pragma once

#include <cstdint>

namespace Vertex
{
    typedef int8_t   VxInt8;
    typedef uint8_t  VxUInt8;
    typedef int16_t  VxInt16;
    typedef uint16_t VxUInt16;
    typedef int32_t  VxInt32;
    typedef uint32_t VxUInt32;
    typedef int64_t  VxInt64;
    typedef uint64_t VxUInt64;

    typedef size_t   VxSize;
    typedef ssize_t  VxSSize;
    typedef void*    VxPtr;
    typedef VxUInt8* VxArithPtr; // a pointer that can be used for arithmetic

    typedef float       VxFloat32;
    typedef double      VxFloat64;
    typedef long double VxFloat80;
}
