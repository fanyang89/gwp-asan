# gwp-asan

Independent standalone gwp-asan, from [llvm](https://github.com/llvm/llvm-project)

## What's this

GWP-ASan is a sampled allocator framework that assists in finding use-after-free and heap-buffer-overflow bugs in
**production** environments.

This repo contains a standalone gwp-asan, you can easily use it with CMake.

Check the documents here: [gwp-asan](https://llvm.org/docs/GwpAsan.html)

## Integration

TL;DR read the [example](https://github.com/fanyang89/gwp-asan/blob/main/example/helloworld/main.cc)

Integration by simply performing the following steps:

1. add hook to malloc/free

    ```c++
    void *operator new(std::size_t sz) {
        if (allocator.shouldSample()) {
            return allocator.allocate(sz, MinAlignment);
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
    ```

2. register SEGV handler and init the allocator

    ```c++
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
    ```

## Resolve output

```bash
./buggy-program | ./scripts/symbolize.sh
```
