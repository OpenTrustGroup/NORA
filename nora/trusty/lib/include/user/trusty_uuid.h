// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The UUID that names a Trusty application or the peer of an IPC channel.
// NORA has no TEE, so no call ever fills one in; the type only lets code that
// carries UUIDs around compile.

#pragma once

#include <stdint.h>

typedef struct uuid {
    uint32_t time_low;
    uint16_t time_mid;
    uint16_t time_hi_and_version;
    uint8_t clock_seq_and_node[8];
} uuid_t;
