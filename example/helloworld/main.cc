#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <cstdarg>

#include "gwp_asan/guarded_pool_allocator.h"
#include "gwp_asan/optional/segv_handler.h"
#include "gwp_asan/optional/backtrace.h"

static gwp_asan::GuardedPoolAllocator allocator;

void *operator new(std::size_t sz) {
    if (allocator.shouldSample()) {
        return allocator.allocate(sz);
    }
    return malloc(sz);
}

void operator delete(void *ptr) {
    if (allocator.pointerIsMine(ptr)) {
        allocator.deallocate(ptr);
        return;
    }
    free(ptr);
}

static void PrintfToBuffer(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buf[8192];
    vsnprintf(buf, sizeof(buf), format, args);
    printf("%.*s", sizeof buf, buf);
    va_end(args);
}

int main() {
    gwp_asan::options::Options options;
    options.Enabled = true;
    options.MaxSimultaneousAllocations = 16;
    options.SampleRate = 3;
    options.Backtrace = gwp_asan::backtrace::getBacktraceFunction();
    allocator.init(options);
    gwp_asan::segv_handler::installSignalHandlers(
            &allocator, PrintfToBuffer,
            gwp_asan::backtrace::getPrintBacktraceFunction(),
            gwp_asan::backtrace::getSegvBacktraceFunction(),
            false
    );

    for (size_t i = 0; i < 10; ++i) {
        std::string s = "Hellooooooooooooooo ";
        std::string_view sv = s + "World";
        std::cout << sv << std::endl;
    }

    return 0;
}
