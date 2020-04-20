cmake_minimum_required(VERSION 3.10)

project(glad)

add_library(glad SHARED src/glad.c) # cannot link a static lib into a shared one

target_include_directories(glad PUBLIC include)

install(TARGETS glad
        CONFIGURATIONS Debug
        LIBRARY DESTINATION Debug/bin)

install(TARGETS glad
        CONFIGURATIONS Release
        LIBRARY DESTINATION Release/bin)