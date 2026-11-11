// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// NORA has no TEE, so there is no Trusty hardware RNG; this draws from the
// kernel's CPRNG instead, so that BoringSSL still gets real random bytes.

#include <lib/rng/trusty_rng.h>
#include <uapi/err.h>
#include <zircon/syscalls.h>

int trusty_rng_hw_rand(uint8_t* data, size_t len) {
    // The kernel hands out at most ZX_CPRNG_DRAW_MAX_LEN bytes per call.
    while (len > 0) {
        size_t chunk = len < ZX_CPRNG_DRAW_MAX_LEN ? len : ZX_CPRNG_DRAW_MAX_LEN;
        size_t actual = 0;
        if (zx_cprng_draw(data, chunk, &actual) != ZX_OK || actual != chunk) {
            return ERR_GENERIC;
        }
        data += chunk;
        len -= chunk;
    }
    return NO_ERROR;
}
