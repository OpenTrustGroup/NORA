// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// NORA has no TEE: what garnet calls of the Trusty library, answering that
// there is no Trusty. The _trusty_ calls return ERR_NOT_SUPPORTED, or
// INVALID_IPC_HANDLE where they return a handle, and the zx_trusty_ calls
// return ZX_ERR_NOT_SUPPORTED. No call writes to its output arguments, except
// that _trusty_ioctl() reports the board the build is for.

#include <pal/platform_defs.h>
#include <string.h>
#include <trusty_std.h>
#include <uapi/err.h>
#include <zircon/errors.h>

long _trusty_port_create(const char* path, uint32_t num_recv_bufs,
                         uint32_t recv_buf_size, uint32_t flags) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_connect(const char* path, uint32_t flags) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_accept(uint32_t handle_id, uuid_t* peer_uuid) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_close(uint32_t handle_id) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_set_cookie(uint32_t handle, void* cookie) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_handle_set_create(void) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_handle_set_ctrl(uint32_t handle, uint32_t cmd, uevent_t* evt) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_wait(uint32_t handle_id, uevent_t* event, uint32_t timeout_msecs) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_wait_any(uevent_t* event, uint32_t timeout_msecs) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_get_msg(uint32_t handle, ipc_msg_info_t* msg_info) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_read_msg(uint32_t handle, uint32_t msg_id, uint32_t offset,
                      ipc_msg_t* msg) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_put_msg(uint32_t handle, uint32_t msg_id) {
    return ERR_NOT_SUPPORTED;
}

long _trusty_send_msg(uint32_t handle, ipc_msg_t* msg) {
    return ERR_NOT_SUPPORTED;
}

// Garnet asks the TEE which board it runs on; with no TEE to ask, the build
// says (NORA_BOARD_PRODUCT_ID, from the nora_board_product_id argument).
long _trusty_ioctl(uint32_t fd, uint32_t req, void* buf) {
    if (fd == SYS_PLATFORM_FD && req == SYS_PLATFORM_GET_PRODUCT_ID && buf != NULL) {
        uint32_t product_id = NORA_BOARD_PRODUCT_ID;
        memcpy(buf, &product_id, sizeof(product_id));
        return NO_ERROR;
    }
    return ERR_NOT_SUPPORTED;
}

long _trusty_mmap(void* uaddr, uint32_t size, uint32_t flags, uint32_t handle) {
    // Callers treat a zero address as failure.
    return 0;
}

long _trusty_munmap(void* uaddr, uint32_t size) {
    return ERR_NOT_SUPPORTED;
}

handle_t _trusty_mref_create_handle(const struct iovec* iov, uint32_t iov_cnt,
                                    uint64_t flags) {
    return INVALID_IPC_HANDLE;
}

handle_t _trusty_mref_create_child(handle_t parent, size_t offset, size_t size,
                                   uint64_t flags) {
    return INVALID_IPC_HANDLE;
}

zx_status_t zx_trusty_create_heap_vmo(bool ion, uint64_t base, uint64_t size,
                                      zx_handle_t* out_vmo) {
    return ZX_ERR_NOT_SUPPORTED;
}

zx_status_t zx_trusty_destroy_heap_vmo(zx_handle_t vmo) {
    return ZX_ERR_NOT_SUPPORTED;
}

zx_status_t zx_trusty_set_config(zx_handle_t process, const void* uuid,
                                 size_t uuid_len, const void* configs,
                                 size_t config_len, int32_t irq) {
    return ZX_ERR_NOT_SUPPORTED;
}

// GRT's C library can trace heap allocations to find leaks in a Trusty
// service; the C library here cannot, so these do nothing and report no
// leaks.
void nbl_start_trace_heap(void) {}

int nbl_end_trace_heap(int try_fix) {
    return 0;
}
