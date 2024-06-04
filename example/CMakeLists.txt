add_executable(helloworld helloworld/main.cc)

target_link_libraries(helloworld PRIVATE
    RTGwpAsan RTGwpAsanSegvHandler RTGwpAsanBacktraceLibc)
target_link_libraries(helloworld PRIVATE jemalloc)
target_compile_options(helloworld PRIVATE -fno-omit-frame-pointer)
target_include_directories(helloworld PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
