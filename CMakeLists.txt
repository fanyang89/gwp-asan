cmake_minimum_required(VERSION 3.17)
project(gwp-asan)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#add_subdirectory(sanitizer_common)
add_subdirectory(gwp_asan)
add_subdirectory(example)
