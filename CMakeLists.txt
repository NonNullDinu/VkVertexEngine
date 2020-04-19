cmake_minimum_required(VERSION 3.10)

project(VertexEngine)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include(Vertex/CMakeLists.txt)
include(SandBox/CMakeLists.txt)