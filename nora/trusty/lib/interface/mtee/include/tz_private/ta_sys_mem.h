// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The MTEE shared-memory queries a trusted application makes of the TEE.
// NORA has no TEE, so the stand-ins in mtee_api answer
// TZ_RESULT_ERROR_NOT_SUPPORTED and leave their output untouched.

#pragma once

#include <stdint.h>

#include <zircon/compiler.h>

__BEGIN_CDECLS

typedef int TZ_RESULT;

#define TZ_RESULT_SUCCESS 0
#define TZ_RESULT_ERROR_GENERIC (-1)
#define TZ_RESULT_ERROR_NOT_SUPPORTED (-3)

// A shared-memory region as the TEE describes it.
typedef struct {
    void* buffer;
    uint32_t size;
} MTEE_SHAREDMEM_PARAM;

// Looks up the shared memory behind |handle|.
TZ_RESULT TA_Mem_QuerySharedmem(uint32_t handle, MTEE_SHAREDMEM_PARAM* param);

// Unmaps a region mapped by TA_Mem_QuerySharedmem().
TZ_RESULT MTEE_UnmmapRegion(void* va, unsigned long size);

__END_CDECLS
