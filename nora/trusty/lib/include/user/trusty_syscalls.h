// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The Trusty system calls, under the _trusty_ names that code running on
// Zircon uses, and the Zircon calls that set up Trusty applications. NORA has
// no TEE: every call here is a stand-in in libc-trusty. The _trusty_ calls
// return ERR_NOT_SUPPORTED, or INVALID_IPC_HANDLE where they return a handle;
// the zx_trusty_ calls return ZX_ERR_NOT_SUPPORTED. None of them writes to
// its output arguments.

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/uio.h>
#include <zircon/compiler.h>
#include <zircon/types.h>

#include <trusty_ipc.h>
#include <trusty_uuid.h>

__BEGIN_CDECLS

// Flags for _trusty_mmap(), _trusty_mref_create_handle() and
// _trusty_mref_create_child().
#define MMAP_FLAG_PROT_NONE 0x0
#define MMAP_FLAG_PROT_READ 0x1
#define MMAP_FLAG_PROT_WRITE 0x2
#define MMAP_FLAG_PROT_EXEC 0x4
#define MMAP_FLAG_PROT_RO MMAP_FLAG_PROT_READ
#define MMAP_FLAG_PROT_RW (MMAP_FLAG_PROT_READ | MMAP_FLAG_PROT_WRITE)
#define MMAP_FLAG_IO_HANDLE 0x10
#define MMAP_FLAG_MEMREF_HANDLE 0x20

long _trusty_port_create(const char* path, uint32_t num_recv_bufs,
                         uint32_t recv_buf_size, uint32_t flags);
long _trusty_connect(const char* path, uint32_t flags);
long _trusty_accept(uint32_t handle_id, uuid_t* peer_uuid);
long _trusty_close(uint32_t handle_id);
long _trusty_set_cookie(uint32_t handle, void* cookie);
long _trusty_handle_set_create(void);
long _trusty_handle_set_ctrl(uint32_t handle, uint32_t cmd, uevent_t* evt);
long _trusty_wait(uint32_t handle_id, uevent_t* event, uint32_t timeout_msecs);
long _trusty_wait_any(uevent_t* event, uint32_t timeout_msecs);
long _trusty_get_msg(uint32_t handle, ipc_msg_info_t* msg_info);
long _trusty_read_msg(uint32_t handle, uint32_t msg_id, uint32_t offset,
                      ipc_msg_t* msg);
long _trusty_put_msg(uint32_t handle, uint32_t msg_id);
long _trusty_send_msg(uint32_t handle, ipc_msg_t* msg);

long _trusty_ioctl(uint32_t fd, uint32_t req, void* buf);

long _trusty_mmap(void* uaddr, uint32_t size, uint32_t flags, uint32_t handle);
long _trusty_munmap(void* uaddr, uint32_t size);
handle_t _trusty_mref_create_handle(const struct iovec* iov, uint32_t iov_cnt,
                                    uint64_t flags);
handle_t _trusty_mref_create_child(handle_t parent, size_t offset, size_t size,
                                   uint64_t flags);

zx_status_t zx_trusty_create_heap_vmo(bool ion, uint64_t base, uint64_t size,
                                      zx_handle_t* out_vmo);
zx_status_t zx_trusty_destroy_heap_vmo(zx_handle_t vmo);
zx_status_t zx_trusty_set_config(zx_handle_t process, const void* uuid,
                                 size_t uuid_len, const void* configs,
                                 size_t config_len, int32_t irq);

__END_CDECLS
