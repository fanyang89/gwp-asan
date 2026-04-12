//===-- harness.cpp ---------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "gwp_asan/tests/harness.h"

#include <string>

#if defined(__clang__)
#define GWP_ASAN_TEST_NOINLINE __attribute__((noinline, optnone))
#elif defined(__GNUC__)
#define GWP_ASAN_TEST_NOINLINE __attribute__((noinline))
#else
#define GWP_ASAN_TEST_NOINLINE
#endif

// Keep these calls visible in backtraces.
GWP_ASAN_TEST_NOINLINE char *
AllocateMemory(gwp_asan::GuardedPoolAllocator &GPA) {
  return static_cast<char *>(GPA.allocate(1));
}
GWP_ASAN_TEST_NOINLINE void
DeallocateMemory(gwp_asan::GuardedPoolAllocator &GPA, void *Ptr) {
  GPA.deallocate(Ptr);
}
GWP_ASAN_TEST_NOINLINE void
DeallocateMemory2(gwp_asan::GuardedPoolAllocator &GPA, void *Ptr) {
  GPA.deallocate(Ptr);
}
GWP_ASAN_TEST_NOINLINE void TouchMemory(void *Ptr) {
  *(reinterpret_cast<volatile char *>(Ptr)) = 7;
}

#undef GWP_ASAN_TEST_NOINLINE

void CheckOnlyOneGwpAsanCrash(const std::string &OutputBuffer) {
  const char *kGwpAsanErrorString = "GWP-ASan detected a memory error";
  size_t FirstIndex = OutputBuffer.find(kGwpAsanErrorString);
  ASSERT_NE(FirstIndex, std::string::npos) << "Didn't detect a GWP-ASan crash";
  ASSERT_EQ(OutputBuffer.find(kGwpAsanErrorString, FirstIndex + 1),
            std::string::npos)
      << "Detected more than one GWP-ASan crash:\n"
      << OutputBuffer;
}

// Fuchsia does not support recoverable GWP-ASan.
#if defined(__Fuchsia__)
INSTANTIATE_TEST_SUITE_P(RecoverableAndNonRecoverableTests,
                         BacktraceGuardedPoolAllocatorDeathTest,
                         /* Recoverable */ testing::Values(false));
#else
INSTANTIATE_TEST_SUITE_P(RecoverableTests, BacktraceGuardedPoolAllocator,
                         /* Recoverable */ testing::Values(true));
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(BacktraceGuardedPoolAllocator);
INSTANTIATE_TEST_SUITE_P(RecoverableAndNonRecoverableTests,
                         BacktraceGuardedPoolAllocatorDeathTest,
                         /* Recoverable */ testing::Bool());
GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(BacktraceGuardedPoolAllocatorDeathTest);
#endif
