cmake_minimum_required(VERSION 3.10)

project(SandBox)

file(GLOB_RECURSE SANDBOX_SOURCES *.cpp)
file(GLOB_RECURSE SANDBOX_HEADERS *.h)

add_executable(SandBox
        ${SANDBOX_SOURCES}
        ${SANDBOX_HEADERS}
        )

target_include_directories(SandBox PRIVATE ${VX_INCLUDE_DIRECTORIES})

target_link_libraries(SandBox Vertex)

install(TARGETS Vertex
        CONFIGURATIONS Debug
        LIBRARY DESTINATION Debug/bin)

install(TARGETS Vertex
        CONFIGURATIONS Release
        LIBRARY DESTINATION Release/bin)
