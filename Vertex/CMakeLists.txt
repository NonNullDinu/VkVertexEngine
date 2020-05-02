cmake_minimum_required(VERSION 3.16)

project(Vertex CXX)

file(GLOB_RECURSE VX_SOURCES
        src/Core/*.cpp
        src/Core/Layer/*.cpp
        src/Renderer/*.cpp
        src/ImGui/*.cpp)

file(GLOB_RECURSE VX_HEADERS
        src/Core/*.h
        src/Core/Layer/*.h
        src/Renderer/*.h
        src/ImGui/*.h

        deps/glm/glm/*.hpp)
if ($ENV{USE_VK})
    file(GLOB_RECURSE VX_GRAPHICS_SOURCES src/GL/Vulkan/*.cpp)
    file(GLOB_RECURSE VX_GRAPHICS_HEADERS src/GL/Vulkan/*.h)

    set(IMGUI_IMPL_HEADERS
            deps/imgui/examples/imgui_impl_glfw.h
            deps/imgui/examples/imgui_impl_vulkan.h)
    set(IMGUI_IMPL_SOURCES
            deps/imgui/examples/imgui_impl_glfw.cpp
            deps/imgui/examples/imgui_impl_vulkan.cpp)
else () # OpenGL
    file(GLOB_RECURSE VX_GRAPHICS_SOURCES src/GL/OpenGL/*.cpp)
    file(GLOB_RECURSE VX_GRAPHICS_HEADERS src/GL/OpenGL/*.h)

    set(IMGUI_IMPL_HEADERS
            deps/imgui/examples/imgui_impl_glfw.h
            deps/imgui/examples/imgui_impl_opengl3.h)
    set(IMGUI_IMPL_SOURCES
            deps/imgui/examples/imgui_impl_glfw.cpp
            deps/imgui/examples/imgui_impl_opengl3.cpp)
endif ()

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
    if ($ENV{USE_VK})
        find_package(Vulkan REQUIRED) # Vulkan
        set(GRAPHICS_LIB_INCLUDES ${Vulkan_INCLUDE_DIRS})
        set(GRAPHICS_LIBS ${Vulkan_LIBRARIES})
    else ()
        find_package(OpenGL REQUIRED) # OpenGL
        set(GRAPHICS_LIB_INCLUDES ${OPENGL_INCLUDE_DIR})
        set(GRAPHICS_LIBS ${OPENGL_LIBRARIES})
    endif ()

    find_package(glfw3 REQUIRED) # GLFW
    set(GLFW_LIBRARY glfw) # FindGLFW.cmake does not set this so we need to do it

    # same explanation as above, but for linux
    set(PLATFORM_SPECIFIC_INCLUDES ${GRAPHICS_LIB_INCLUDES} ${GLEW_INCLUDE_DIRS})
    set(PLATFORM_SPECIFIC_LIBS ${GRAPHICS_LIBS} ${GLFW_LIBRARY})
else ()
    message(FATAL_ERROR Unsupported platform)
endif ()

add_library(Vertex SHARED
        ${VX_HEADERS}
        ${VX_SOURCES}

        ${IMGUI_IMPL_HEADERS}
        ${IMGUI_IMPL_SOURCES}

        ${VX_PLATFORM_HEADERS}
        ${VX_PLATFORM_SOURCES}

        ${VX_GRAPHICS_HEADERS}
        ${VX_GRAPHICS_SOURCES})

# add the paths in PLATFORM_SPECIFIC_INCLUDES to Vertex's include directories
target_include_directories(Vertex
        PUBLIC src deps/spdlog/include deps/imgui deps/glm ${PLATFORM_SPECIFIC_INCLUDES})

# use the libs specified above in PLATFORM_SPECIFIC_LIBS and glad
target_link_libraries(Vertex
        PUBLIC ${PLATFORM_SPECIFIC_LIBS} glad imgui)

set_target_properties(Vertex PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(Vertex PROPERTIES PUBLIC_HEADER include/Vertex.h)

# this is going to be exported so we can use it in /CMakeLists.txt and /SandBox/CMakeLists.txt
set(VX_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

target_precompile_headers(Vertex PUBLIC src/Core/vx_pch.h)

if ($ENV{USE_VK})
    add_compile_definitions(VX_RENDER_API_VULKAN)
else ()
    target_compile_definitions(Vertex PRIVATE VX_BUILD_SHARED IMGUI_IMPL_OPENGL_LOADER_GLAD=1)
    add_compile_definitions(VX_RENDER_API_OPENGL)
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(Vertex PRIVATE VX_TARGET_DEBUG=1)
else ()
    target_compile_definitions(Vertex PRIVATE VX_TARGET_DEBUG=0)
endif()


if ($ENV{CLION_IDE})
    target_compile_options(Vertex PRIVATE -include Core/vx_pch.h)
endif ()

install(TARGETS Vertex
        CONFIGURATIONS Debug
        LIBRARY DESTINATION Debug/bin)

install(TARGETS Vertex
        CONFIGURATIONS Release
        LIBRARY DESTINATION Release/bin)