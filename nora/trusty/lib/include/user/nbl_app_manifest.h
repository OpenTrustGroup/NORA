// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// What the manifest of a Nebula Trusty application tells the application
// manager: its UUID and how to launch it. NORA has no TEE and runs no Trusty
// applications, so nothing ever sets these flags; zx_trusty_set_config(),
// which applies a manifest to a process, answers ZX_ERR_NOT_SUPPORTED. The
// flag values are NORA's own.

#pragma once

#include <stdint.h>

#include <trusty_syscalls.h>
#include <trusty_uuid.h>

// Flags in an application package's trusty_flags.
#define NEBULA_APP_FLAGS_RESTART_ON_EXIT (1u << 0)
#define NEBULA_APP_FLAGS_NEW_PROCESS (1u << 1)
#define NEBULA_APP_FLAGS_CALL_TRUSTY_API (1u << 2)
#define NEBULA_APP_FLAGS_TUI (1u << 3)
