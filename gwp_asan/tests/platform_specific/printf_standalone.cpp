//===-- printf_standalone.cpp ----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "gwp_asan/optional/printf.h"

#include <stdarg.h>
#include <stdio.h>

namespace gwp_asan {
namespace test {
namespace {

void TestPrintf(const char *Format, ...) {
  va_list AP;
  va_start(AP, Format);
  vfprintf(stderr, Format, AP);
  va_end(AP);
}

} // anonymous namespace

Printf_t getPrintfFunction() { return TestPrintf; }

} // namespace test
} // namespace gwp_asan
