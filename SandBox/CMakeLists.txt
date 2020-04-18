cmake_minimum_required(VERSION 3.10)

project(SandBox)

add_executable(SandBox 
    SandBox/src/Application.cpp
)

target_include_directories(SandBox PRIVATE Vertex/include)
target_link_directories(SandBox PRIVATE build/Debug)

target_link_libraries(SandBox Vertex)
