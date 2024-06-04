cmake_minimum_required(VERSION 3.17)
project(gwp-asan)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(BUILD_EXAMPLE "Build examples" OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(gwp_asan)

if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif ()
