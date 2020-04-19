cmake_minimum_required(VERSION 3.10)

project(Vertex)

add_library(Vertex SHARED
    ${PROJECT_SOURCE_DIR}/Vertex/src/Core/Application.cpp
    ${PROJECT_SOURCE_DIR}/Vertex/src/Core/Logger.cpp
)

target_include_directories(Vertex 
    PUBLIC ${PROJECT_SOURCE_DIR}/Vertex/src
    PUBLIC ${PROJECT_SOURCE_DIR}/Vertex/deps/spdlog/include
)

set_target_properties(Vertex PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(Vertex PROPERTIES PUBLIC_HEADER ${PROJECT_SOURCE_DIR}/include/Vertex.h)

target_precompile_headers(Vertex PUBLIC ${PROJECT_SOURCE_DIR}/Vertex/src/Core/vx_pch.h)

target_compile_definitions(Vertex PRIVATE VX_BUILD_SHARED)

install(TARGETS Vertex
    CONFIGURATIONS Debug
    LIBRARY DESTINATION Debug/bin)

install(TARGETS Vertex
    CONFIGURATIONS Release
    LIBRARY DESTINATION Release/bin)