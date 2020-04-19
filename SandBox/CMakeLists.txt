cmake_minimum_required(VERSION 3.10)

project(SandBox)

add_executable(SandBox 
    src/Application.cpp
)
message(STATUS ${VX_INCLUDE_DIRECTORIES})
target_include_directories(SandBox PRIVATE ${VX_INCLUDE_DIRECTORIES})

target_link_libraries(SandBox Vertex)

install(TARGETS Vertex
    CONFIGURATIONS Debug
    LIBRARY DESTINATION Debug/bin)

install(TARGETS Vertex
    CONFIGURATIONS Release
    LIBRARY DESTINATION Release/bin)
