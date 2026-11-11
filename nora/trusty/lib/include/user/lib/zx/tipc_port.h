// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The C++ wrapper of a Trusty IPC port handle. NORA has no TEE, so no port
// can exist: create() fails with ZX_ERR_NOT_SUPPORTED and leaves the output
// invalid, and accept() fails with ZX_ERR_NOT_SUPPORTED. Everything is
// inline, so users need not link a Trusty library.

#pragma once

#include <lib/zx/handle.h>
#include <lib/zx/object.h>
#include <lib/zx/tipc_chan.h>
#include <zircon/errors.h>
#include <zircon/types.h>

#include <trusty_ipc.h>
#include <trusty_uuid.h>

namespace zx {

class tipc_port : public object<tipc_port> {
public:
    constexpr tipc_port() = default;

    explicit tipc_port(zx_handle_t value) : object(value) {}

    explicit tipc_port(handle&& h) : object(h.release()) {}

    tipc_port(tipc_port&& other) : object(other.release()) {}

    tipc_port& operator=(tipc_port&& other) {
        reset(other.release());
        return *this;
    }

    // Publishes a port named by the first |len| bytes of |name|. There is no
    // Trusty to publish it in.
    static zx_status_t create(uint32_t flags, const char* name, size_t len,
                              tipc_port* result) {
        return ZX_ERR_NOT_SUPPORTED;
    }

    zx_status_t accept(uuid_t* peer_uuid, tipc_chan* chan) const {
        return ZX_ERR_NOT_SUPPORTED;
    }
};

}  // namespace zx
