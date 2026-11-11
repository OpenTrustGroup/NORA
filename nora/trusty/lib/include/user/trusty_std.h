// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The Trusty user library's standard header: the IPC types, the system calls
// and the platform requests. NORA has no TEE, so every call it declares is a
// stand-in that reports that there is no Trusty; see trusty_syscalls.h.

#pragma once

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/uio.h>

#include <trusty_ipc.h>
#include <trusty_sys_platform.h>
#include <trusty_syscalls.h>
#include <trusty_uuid.h>
