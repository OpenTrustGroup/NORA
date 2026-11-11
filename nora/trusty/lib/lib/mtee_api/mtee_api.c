// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// NORA has no TEE: the MTEE calls answer TZ_RESULT_ERROR_NOT_SUPPORTED and
// leave their output untouched.

#include <tz_private/ta_sys_mem.h>

TZ_RESULT TA_Mem_QuerySharedmem(uint32_t handle, MTEE_SHAREDMEM_PARAM* param) {
    return TZ_RESULT_ERROR_NOT_SUPPORTED;
}

TZ_RESULT MTEE_UnmmapRegion(void* va, unsigned long size) {
    return TZ_RESULT_ERROR_NOT_SUPPORTED;
}
