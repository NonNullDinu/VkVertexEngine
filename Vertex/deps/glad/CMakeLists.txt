cmake_minimum_required(VERSION 3.10)

project(glad)

add_library(glad STATIC src/glad.c)

target_link_libraries(glad PRIVATE dl)

set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(glad PUBLIC include)

install(TARGETS glad
        CONFIGURATIONS Debug
        LIBRARY DESTINATION Debug/bin)

install(TARGETS glad
        CONFIGURATIONS Release
        LIBRARY DESTINATION Release/bin)