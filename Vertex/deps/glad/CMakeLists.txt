cmake_minimum_required(VERSION 3.10)

project(glad)

add_library(glad STATIC src/glad.c) # cannot link a static lib into a shared one

target_include_directories(glad PUBLIC include)
set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)

install(TARGETS glad
        CONFIGURATIONS Debug
        LIBRARY DESTINATION Debug/bin)

install(TARGETS glad
        CONFIGURATIONS Release
        LIBRARY DESTINATION Release/bin)