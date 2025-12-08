cmake_minimum_required(VERSION 3.15...4.0)
project(walk)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor)
add_subdirectory(src)
