set(GWP_ASAN_UNITTESTS
  platform_specific/printf_standalone.cpp
  alignment.cpp
  backtrace.cpp
  basic.cpp
  compression.cpp
  iterate.cpp
  crash_handler_api.cpp
  driver.cpp
  mutex_test.cpp
  slot_reuse.cpp
  thread_contention.cpp
  harness.cpp
  enable_disable.cpp
  late_init.cpp
  options.cpp
  recoverable.cpp
  never_allocated.cpp
  utilities.cpp
)

if (GWP_ASAN_GTEST_SOURCE_DIR)
  set(GWP_ASAN_GTEST_ROOT "${GWP_ASAN_GTEST_SOURCE_DIR}")
  if (EXISTS "${GWP_ASAN_GTEST_ROOT}/googletest")
    set(GWP_ASAN_GTEST_ROOT "${GWP_ASAN_GTEST_ROOT}/googletest")
  endif ()
  if (NOT EXISTS "${GWP_ASAN_GTEST_ROOT}/include/gtest/gtest.h" OR
      NOT EXISTS "${GWP_ASAN_GTEST_ROOT}/src/gtest-all.cc")
    message(FATAL_ERROR
      "GWP_ASAN_GTEST_SOURCE_DIR must point to a googletest checkout")
  endif ()

  add_library(gwp_asan_gtest STATIC
    "${GWP_ASAN_GTEST_ROOT}/src/gtest-all.cc"
  )
  target_include_directories(gwp_asan_gtest PUBLIC
    "${GWP_ASAN_GTEST_ROOT}/include"
    "${GWP_ASAN_GTEST_ROOT}"
  )
  set(GWP_ASAN_GTEST_TARGET gwp_asan_gtest)
else ()
  find_package(GTest REQUIRED)
  if (TARGET GTest::gtest)
    set(GWP_ASAN_GTEST_TARGET GTest::gtest)
  elseif (TARGET GTest::GTest)
    set(GWP_ASAN_GTEST_TARGET GTest::GTest)
  else ()
    message(FATAL_ERROR "Could not find a usable GTest target")
  endif ()
endif ()

add_executable(gwp_asan_tests
  ${GWP_ASAN_UNITTESTS}
)

target_link_libraries(gwp_asan_tests PRIVATE
  RTGwpAsan
  RTGwpAsanOptionsParser
  RTGwpAsanBacktraceLibc
  RTGwpAsanSegvHandler
  ${GWP_ASAN_GTEST_TARGET}
)

target_compile_features(gwp_asan_tests PRIVATE cxx_std_17)
target_compile_options(gwp_asan_tests PRIVATE
  -O0
  -g
  -fno-omit-frame-pointer
  -fno-optimize-sibling-calls
)

if (NOT WIN32)
  target_link_libraries(gwp_asan_tests PRIVATE dl pthread)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_link_options(gwp_asan_tests PRIVATE -rdynamic)
endif ()

include(GoogleTest)
gtest_discover_tests(gwp_asan_tests)
