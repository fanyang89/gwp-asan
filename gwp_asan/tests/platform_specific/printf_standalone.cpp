//===-- printf_standalone.cpp ----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "gwp_asan/optional/printf.h"

#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_FLOAT_HEX_FORMAT_SPECIFIER 0
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_ALT_FORM_FLAG 0
#define NANOPRINTF_VISIBILITY_STATIC
#define NANOPRINTF_IMPLEMENTATION
#include "third_party/nanoprintf/nanoprintf.h"

namespace gwp_asan {
namespace test {
namespace {

constexpr size_t kWriteBufferSize = 256;

struct BufferedWriter {
  char Buffer[kWriteBufferSize];
  size_t Length = 0;
};

void writeAll(const char *Data, size_t Length) {
  while (Length > 0) {
    ssize_t Written = write(STDERR_FILENO, Data, Length);
    if (Written > 0) {
      Data += Written;
      Length -= static_cast<size_t>(Written);
      continue;
    }

    if (Written < 0 && errno == EINTR)
      continue;

    return;
  }
}

void flushBufferedWriter(BufferedWriter *Writer) {
  writeAll(Writer->Buffer, Writer->Length);
  Writer->Length = 0;
}

void bufferedPutc(int C, void *Ctx) {
  auto *Writer = static_cast<BufferedWriter *>(Ctx);
  Writer->Buffer[Writer->Length++] = static_cast<char>(C);
  if (Writer->Length == sizeof(Writer->Buffer))
    flushBufferedWriter(Writer);
}

void TestPrintf(const char *Format, ...) {
  BufferedWriter Writer = {};
  va_list AP;
  va_start(AP, Format);
  npf_vpprintf(bufferedPutc, &Writer, Format, AP);
  va_end(AP);

  flushBufferedWriter(&Writer);
}

} // anonymous namespace

Printf_t getPrintfFunction() { return TestPrintf; }

} // namespace test
} // namespace gwp_asan
