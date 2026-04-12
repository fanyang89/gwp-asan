cmake_minimum_required(VERSION 3.17)
project(gwp-asan)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (DEFINED BUILD_EXAMPLE AND NOT DEFINED GWP_ASAN_BUILD_EXAMPLE)
    set(GWP_ASAN_BUILD_EXAMPLE "${BUILD_EXAMPLE}" CACHE BOOL "Build examples")
endif ()
if (DEFINED BUILD_EXAMPLE)
    message(DEPRECATION
        "BUILD_EXAMPLE is deprecated; use GWP_ASAN_BUILD_EXAMPLE instead.")
endif ()

option(GWP_ASAN_BUILD_EXAMPLE "Build examples" OFF)
option(GWP_ASAN_BUILD_TESTING "Build tests" OFF)
set(GWP_ASAN_GTEST_SOURCE_DIR "" CACHE PATH
    "Path to a googletest checkout used when GTest is not installed")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if (GWP_ASAN_BUILD_TESTING)
    enable_testing()
endif ()

add_subdirectory(gwp_asan)

if (GWP_ASAN_BUILD_EXAMPLE)
    add_subdirectory(example)
endif ()
