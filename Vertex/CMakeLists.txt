cmake_minimum_required(VERSION 3.10)

project(Vertex)

add_library(Vertex SHARED
    Vertex/src/Application.cpp
)

set_property(TARGET Vertex PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties(Vertex PROPERTIES PUBLIC_HEADER include/Vertex.h)

target_include_directories(Vertex PRIVATE Vertex/src)

if (MSVC)
    set_target_properties(Vertex 
        PROPERTIES COMPILE_DEFINITIONS VX_BUILD_SHARED
    )
endif (MSVC)

install(TARGETS Vertex
    CONFIGURATIONS Debug
    RUNTIME DESTINATION Debug/bin)

install(TARGETS Vertex
    CONFIGURATIONS Release
    RUNTIME DESTINATION Release/bin)