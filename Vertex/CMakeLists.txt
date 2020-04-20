cmake_minimum_required(VERSION 3.16)

project(Vertex CXX)

file(GLOB_RECURSE VX_SOURCES src/Core/*.cpp)
file(GLOB_RECURSE VX_HEADERS include/Core/*.h src/Core/*.h)


# GLAD
add_subdirectory(deps/glad)

if (WIN32)
    file(GLOB_RECURSE VX_PLATFORM_HEADERS src/Platforms/Windows/*.h)
    file(GLOB_RECURSE VX_PLATFORM_SOURCES src/Platforms/Windows/*.cpp)

    add_subdirectory(deps/glfw)

    set(PLATFORM_SPECIFIC_INCLUDES deps/glad/include deps/glfw/include ${GLFW_INCLUDE_DIRS})
    set(PLATFORM_SPECIFIC_LIBS glfw)
elseif (UNIX AND NOT APPLE)
    file(GLOB_RECURSE VX_PLATFORM_HEADERS src/Platforms/Linux/*.h)
    file(GLOB_RECURSE VX_PLATFORM_SOURCES src/Platforms/Linux/*.cpp)
    add_compile_definitions(GLFW_INCLUDE_NONE) # required to use GLEW

    # OpenGL
    find_package(OpenGL REQUIRED)

    # GLFW
    find_package(glfw3 REQUIRED)
    set(GLFW_LIBRARY glfw)

    set(PLATFORM_SPECIFIC_INCLUDES ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
    set(PLATFORM_SPECIFIC_LIBS ${OPENGL_LIBRARIES} ${GLFW_LIBRARY})
else ()
    message(FATAL_ERROR Unsupported platform)
endif ()

add_library(Vertex SHARED
        ${VX_SOURCES}
        ${VX_HEADERS}
        ${VX_PLATFORM_HEADERS}
        ${VX_PLATFORM_SOURCES})

target_include_directories(Vertex
        PUBLIC src deps/spdlog/include ${PLATFORM_SPECIFIC_INCLUDES})

target_link_libraries(Vertex
        PUBLIC ${PLATFORM_SPECIFIC_LIBS} glad
        )

set_target_properties(Vertex PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(Vertex PROPERTIES PUBLIC_HEADER include/Vertex.h)

set(VX_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

target_precompile_headers(Vertex PUBLIC src/Core/vx_pch.h)

target_compile_definitions(Vertex PRIVATE VX_BUILD_SHARED)

install(TARGETS Vertex
        CONFIGURATIONS Debug
        LIBRARY DESTINATION Debug/bin)

install(TARGETS Vertex
        CONFIGURATIONS Release
        LIBRARY DESTINATION Release/bin)