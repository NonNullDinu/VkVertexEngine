cmake_minimum_required(VERSION 3.10)

project(Vertex)

add_library(Vertex SHARED
    Vertex/src/Core/Application.cpp
    Vertex/src/Core/Logger.cpp
)

target_include_directories(Vertex 
    PUBLIC Vertex/src
    PUBLIC Vertex/deps/spdlog/include
)

set_target_properties(Vertex PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(Vertex PROPERTIES PUBLIC_HEADER include/Vertex.h)

target_precompile_headers(Vertex PUBLIC Vertex/src/Core/vx_pch.h)

target_compile_definitions(Vertex PRIVATE VX_BUILD_SHARED)

install(TARGETS Vertex
    CONFIGURATIONS Debug
    LIBRARY DESTINATION Debug/bin)

install(TARGETS Vertex
    CONFIGURATIONS Release
    LIBRARY DESTINATION Release/bin)