cmake_minimum_required(VERSION 3.16)

project(SandBox CXX)

# Get all sources and headers in SANDBOX_SOURCES and SANDBOX_HEADERS respectively

file(GLOB_RECURSE SANDBOX_SOURCES *.cpp)
file(GLOB_RECURSE SANDBOX_HEADERS *.h)

# Add them to the executable
add_executable(SandBox
        ${SANDBOX_SOURCES}
        ${SANDBOX_HEADERS}
        )

# Add the VX_INCLUDE_DIRECTORIES exported from /Vertex/CMakeLists.txt to SandBox's include directories
target_include_directories(SandBox PRIVATE ${VX_INCLUDE_DIRECTORIES})

# Add Vertex as a library for SandBox
target_link_libraries(SandBox Vertex)

install(TARGETS Vertex
        CONFIGURATIONS Debug
        LIBRARY DESTINATION Debug/bin)

install(TARGETS Vertex
        CONFIGURATIONS Release
        LIBRARY DESTINATION Release/bin)
