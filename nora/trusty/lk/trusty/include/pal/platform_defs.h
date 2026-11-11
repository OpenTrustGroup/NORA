// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The platform definitions shared by the Trusty kernel and its users: board
// identifiers and the arguments of platform requests. NORA has no TEE, so
// _trusty_ioctl() answers SYS_PLATFORM_GET_PRODUCT_ID with the board the build
// is for and every other request with ERR_NOT_SUPPORTED. The values are
// NORA's own and are only distinct from each other.

#pragma once

#include <stdint.h>

#include <trusty_sys_platform.h>

// Board identifiers returned by SYS_PLATFORM_GET_PRODUCT_ID.
#define BOARD_VID_QEMU 0x0001u
#define BOARD_PID_MTK_MT8668_EVB 0x0002u

// The argument of SYS_PLATFORM_IRQ_MONITOR_CONFIG: a bitmap of the monitored
// interrupts and the per-interrupt count that triggers a report.
struct irq_monitor_cfg {
    uint8_t enable;
    uint32_t irq_cnt_threshold;
    uint64_t irqs[16];
};
