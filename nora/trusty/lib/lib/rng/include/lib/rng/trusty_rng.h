// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <zircon/compiler.h>

__BEGIN_CDECLS

// Fills |data| with |len| random bytes from the kernel's CPRNG; returns
// NO_ERROR.
int trusty_rng_hw_rand(uint8_t* data, size_t len);

__END_CDECLS
