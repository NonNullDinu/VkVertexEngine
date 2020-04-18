cmake_minimum_required(VERSION 3.10)

project(Vertex)

add_library(Vertex SHARED Vertex/src/Application.cpp)

set_target_properties(Vertex PROPERTIES PUBLIC_HEADER include/Vertex.h)

target_include_directories(Vertex PRIVATE Vertex/src)

install(TARGETS Vertex
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
)