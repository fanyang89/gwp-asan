add_executable(helloworld helloworld/main.cc)
target_link_libraries(helloworld PRIVATE RTGwpAsan RTGwpAsanSegvHandler)
target_include_directories(helloworld PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../)
