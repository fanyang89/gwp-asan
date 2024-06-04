set(GWP_ASAN_SOURCES
    common.cpp
    crash_handler.cpp
    platform_specific/common_posix.cpp
    platform_specific/guarded_pool_allocator_posix.cpp
    platform_specific/mutex_posix.cpp
    platform_specific/utilities_posix.cpp
    guarded_pool_allocator.cpp
    stack_trace_compressor.cpp
)

set(GWP_ASAN_HEADERS
    common.h
    crash_handler.h
    definitions.h
    guarded_pool_allocator.h
    mutex.h
    options.h
    options.inc
    platform_specific/guarded_pool_allocator_fuchsia.h
    platform_specific/guarded_pool_allocator_posix.h
    platform_specific/guarded_pool_allocator_tls.h
    platform_specific/mutex_fuchsia.h
    platform_specific/mutex_posix.h
    stack_trace_compressor.h
    utilities.h
)

# Ensure that GWP-ASan meets the delegated requirements of some supporting
# allocators. Some supporting allocators (e.g. scudo standalone) cannot use any
# parts of the C++ standard library.
set(GWP_ASAN_CFLAGS ${SANITIZER_COMMON_CFLAGS} -fno-rtti -fno-exceptions
    -nostdinc++ -pthread -fno-omit-frame-pointer)
# append_list_if(COMPILER_RT_HAS_FPIC_FLAG -fPIC GWP_ASAN_CFLAGS)
# append_list_if(COMPILER_RT_HAS_SANITIZER_COMMON ${SANITIZER_COMMON_CFLAGS} GWP_ASAN_CFLAGS)

# Remove -stdlib= which is unused when passing -nostdinc++.
string(REGEX REPLACE "-stdlib=[a-zA-Z+]*" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Options parsing support is optional. This is an optional library that can be
# used by an allocator to automatically parse GwpAsan options from the
# environment variable GWP_ASAN_FLAGS, but the allocator can choose to implement
# its own options parsing and populate the Options struct itself.
set(GWP_ASAN_OPTIONS_PARSER_SOURCES
    optional/options_parser.cpp
)
set(GWP_ASAN_OPTIONS_PARSER_HEADERS
    optional/options_parser.h
    options.h
    options.inc
)
set(GWP_ASAN_BACKTRACE_HEADERS
    optional/backtrace.h
    options.h
    options.inc
)
set(GWP_ASAN_SEGV_HANDLER_HEADERS
    optional/segv_handler.h
    options.h)

set(GWP_ASAN_OPTIONS_PARSER_CFLAGS ${GWP_ASAN_CFLAGS})

add_library(RTGwpAsan STATIC
    ${GWP_ASAN_SOURCES}
)

add_library(RTGwpAsanOptionsParser STATIC
    ${GWP_ASAN_OPTIONS_PARSER_SOURCES}
)

# As above, build the pre-implemented optional backtrace support libraries.
add_library(RTGwpAsanBacktraceLibc STATIC
    optional/backtrace_linux_libc.cpp
)

add_library(RTGwpAsanSegvHandler STATIC
    optional/segv_handler_posix.cpp
)

if (COMPILER_RT_INCLUDE_TESTS)
  add_subdirectory(tests)
endif ()
