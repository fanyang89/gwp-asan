//===-- utilities_fuchsia.cpp -----------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "gwp_asan/utilities.h"

#include <alloca.h>
#include <stdio.h>
#include <string.h>
#include <zircon/sanitizer.h>
#include <zircon/status.h>

namespace gwp_asan {
void die(const char *Message) {
  __sanitizer_log_write(Message, strlen(Message));
  __builtin_trap();
}

void dieWithErrorCode(const char *Message, int64_t ErrorCode) {
  const char *ErrorStr =
      _zx_status_get_string(static_cast<zx_status_t>(ErrorCode));
  size_t BufferSize = strlen(Message) + 32 + strlen(ErrorStr);
  char *Buffer = static_cast<char *>(alloca(BufferSize));
  snprintf(Buffer, BufferSize, "%s (Error Code: %s)", Message, ErrorStr);
  __sanitizer_log_write(Buffer, strlen(Buffer));
  __builtin_trap();
}
} // namespace gwp_asan
