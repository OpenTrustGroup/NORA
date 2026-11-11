// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The C++ wrapper of a Trusty IPC channel handle. NORA has no TEE, so no
// channel can exist: create() fails with ZX_ERR_NOT_SUPPORTED and leaves the
// output invalid, and read() and write() fail with ZX_ERR_NOT_SUPPORTED.
// Everything is inline, so users need not link a Trusty library.

#pragma once

#include <lib/zx/handle.h>
#include <lib/zx/object.h>
#include <lib/zx/time.h>
#include <zircon/errors.h>
#include <zircon/types.h>

#include <trusty_ipc.h>

namespace zx {

class tipc_chan : public object<tipc_chan> {
public:
    constexpr tipc_chan() = default;

    explicit tipc_chan(zx_handle_t value) : object(value) {}

    explicit tipc_chan(handle&& h) : object(h.release()) {}

    tipc_chan(tipc_chan&& other) : object(other.release()) {}

    tipc_chan& operator=(tipc_chan&& other) {
        reset(other.release());
        return *this;
    }

    // Connects to the port named by the first |len| bytes of |name|. There is
    // no Trusty to connect to.
    static zx_status_t create(uint32_t flags, const char* name, size_t len,
                              tipc_chan* result) {
        return ZX_ERR_NOT_SUPPORTED;
    }

    zx_status_t write(const void* bytes, uint32_t num_bytes) const {
        return ZX_ERR_NOT_SUPPORTED;
    }

    zx_status_t read(void* bytes, uint32_t num_bytes,
                     uint32_t* actual_bytes = nullptr) const {
        return ZX_ERR_NOT_SUPPORTED;
    }
};

}  // namespace zx
