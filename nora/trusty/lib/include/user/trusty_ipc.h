// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The types and constants of the Trusty IPC interface: handles, messages,
// events and the port and connect flags. NORA has no TEE, so no port or
// channel can ever be opened; the IPC calls in trusty_syscalls.h answer
// ERR_NOT_SUPPORTED and the zx::tipc_chan and zx::tipc_port wrappers answer
// ZX_ERR_NOT_SUPPORTED.

#pragma once

#include <stddef.h>
#include <stdint.h>

#include <trusty_uuid.h>

typedef int32_t handle_t;

#define INVALID_IPC_HANDLE ((handle_t)-1)

#define INFINITE_TIME UINT32_MAX

#define IPC_PORT_PATH_MAX 64

// Flags for _trusty_port_create().
#define IPC_PORT_ALLOW_TA_CONNECT 0x1
#define IPC_PORT_ALLOW_NS_CONNECT 0x2

// Flags for _trusty_connect() and zx::tipc_chan::create().
#define IPC_CONNECT_WAIT_FOR_PORT 0x1
#define IPC_CONNECT_ASYNC 0x2

// Events reported in uevent_t.event. Code that waits on a channel through
// the Zircon wait primitives uses the same values as signals.
#define IPC_HANDLE_POLL_NONE 0x0
#define IPC_HANDLE_POLL_READY 0x1
#define IPC_HANDLE_POLL_ERROR 0x2
#define IPC_HANDLE_POLL_HUP 0x4
#define IPC_HANDLE_POLL_MSG 0x8
#define IPC_HANDLE_POLL_SEND_UNBLOCKED 0x10

// Commands for _trusty_handle_set_ctrl().
#define HSET_ADD 0x0
#define HSET_DEL 0x1
#define HSET_MOD 0x2

typedef struct iovec_trusty {
    void* base;
    size_t len;
} iovec_t;

typedef struct ipc_msg {
    uint32_t num_iov;
    iovec_t* iov;
    uint32_t num_handles;
    handle_t* handles;
} ipc_msg_t;

typedef struct ipc_msg_info {
    size_t len;
    uint32_t id;
    uint32_t num_handles;
} ipc_msg_info_t;

typedef struct uevent {
    handle_t handle;
    uint32_t event;
    void* cookie;
} uevent_t;
