cmake_minimum_required(VERSION 3.16)

project(Vertex CXX)

file(GLOB_RECURSE VX_SOURCES
        src/Core/*.cpp
        src/Core/Layer/*.cpp
        src/Renderer/*.cpp
        src/GL/OpenGL/*.cpp
        src/Window/*.cpp
        src/ImGui/*.cpp)

file(GLOB_RECURSE VX_HEADERS
        src/Core/*.h
        src/Core/Layer/*.h
        src/Renderer/*.h
        src/GL/OpenGL/*.h
        src/Window/*.h
        src/ImGui/*.h
        deps/glm/glm/*.hpp)

# GLAD
add_subdirectory(deps/glad)
# ImGui
add_subdirectory(deps/imgui)

if (WIN32)
    # Get all windows specific headers and sources in VX_PLATFORM_HEADERS and VX_PLATFORM_SOURCES
    file(GLOB_RECURSE VX_PLATFORM_HEADERS src/Platforms/Windows/*.h)
    file(GLOB_RECURSE VX_PLATFORM_SOURCES src/Platforms/Windows/*.cpp)

    # Add glfw
    add_subdirectory(deps/glfw)

    # The include paths specific to building Vertex on windows to be used for target_include_directory
    set(PLATFORM_SPECIFIC_INCLUDES deps/glad/include deps/glfw/include ${GLFW_INCLUDE_DIRS})

    # The libraries specific to building on windows
    set(PLATFORM_SPECIFIC_LIBS glfw)
elseif (UNIX AND NOT APPLE) # e.g. pretty much linux
    # same explanation as above, but for linux
    file(GLOB_RECURSE VX_PLATFORM_HEADERS src/Platforms/Linux/*.h)
    file(GLOB_RECURSE VX_PLATFORM_SOURCES src/Platforms/Linux/*.cpp)

    # find OpenGL and GLFW to get the include paths to use and libraries to link with
    find_package(OpenGL REQUIRED) # OpenGL
    find_package(glfw3 REQUIRED) # GLFW
    set(GLFW_LIBRARY glfw) # FindGLFW.cmake does not set this so we need to do it

    # same explanation as above, but for linux
    set(PLATFORM_SPECIFIC_INCLUDES ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
    set(PLATFORM_SPECIFIC_LIBS ${OPENGL_LIBRARIES} ${GLFW_LIBRARY})
else ()
    message(FATAL_ERROR Unsupported platform)
endif ()

add_library(Vertex SHARED
        ${VX_SOURCES}
        deps/imgui/examples/imgui_impl_glfw.cpp
        deps/imgui/examples/imgui_impl_opengl3.cpp
        ${VX_HEADERS}
        deps/imgui/examples/imgui_impl_glfw.h
        deps/imgui/examples/imgui_impl_opengl3.h

        ${VX_PLATFORM_HEADERS}
        ${VX_PLATFORM_SOURCES})

# add the paths in PLATFORM_SPECIFIC_INCLUDES to Vertex's include directories
target_include_directories(Vertex
        PUBLIC src deps/spdlog/include deps/imgui deps/glm ${PLATFORM_SPECIFIC_INCLUDES})

# use the libs specified above in PLATFORM_SPECIFIC_LIBS and glad
target_link_libraries(Vertex
        PUBLIC ${PLATFORM_SPECIFIC_LIBS} glad imgui
        )

set_target_properties(Vertex PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(Vertex PROPERTIES PUBLIC_HEADER include/Vertex.h)

# this is going to be exported so we can use it in /CMakeLists.txt and /SandBox/CMakeLists.txt
set(VX_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

target_precompile_headers(Vertex PUBLIC src/Core/vx_pch.h)

target_compile_definitions(Vertex PRIVATE VX_BUILD_SHARED IMGUI_IMPL_OPENGL_LOADER_GLAD=1)


if ($ENV{CLION_IDE})
    target_compile_options(Vertex PRIVATE -include Core/vx_pch.h)
endif()

install(TARGETS Vertex
        CONFIGURATIONS Debug
        LIBRARY DESTINATION Debug/bin)

install(TARGETS Vertex
        CONFIGURATIONS Release
        LIBRARY DESTINATION Release/bin)