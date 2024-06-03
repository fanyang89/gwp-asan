#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>

#include "gwp_asan/guarded_pool_allocator.h"

static gwp_asan::GuardedPoolAllocator allocator;

void *operator new(std::size_t sz)
{
    if (allocator.shouldSample())
    {
        return allocator.allocate(sz, 0);
    }
    return malloc(sz);
}

void operator delete(void *ptr)
{
    allocator.deallocate(ptr);
    free(ptr);
}

int *foo()
{
    int x;
    return &x;
}

int main()
{
    std::string s = "Hellooooooooooooooo ";
    std::string_view sv = s + "World\n";
    std::cout << sv;
    return 0;
}
