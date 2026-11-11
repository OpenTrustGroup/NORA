// Copyright 2026 Open Trust Group. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The platform requests that Trusty user code sends with _trusty_ioctl(). NORA
// has no TEE and no Trusty kernel to serve them, so _trusty_ioctl() answers
// every request with ERR_NOT_SUPPORTED and leaves the caller's buffer
// untouched. The numbers are NORA's own; nothing outside this tree reads them.

#pragma once

// The descriptors that _trusty_ioctl() requests are sent to.
#define SYS_PLATFORM_FD 0x100
#define NEBULA_GENERIC_FD 0x101

// Platform identity and configuration.
#define SYS_PLATFORM_GET_VENDOR_ID 1
#define SYS_PLATFORM_GET_PRODUCT_ID 2
#define SYS_PLATFORM_GET_SEC_BOOT 3
#define SYS_PLATFORM_GET_NSS_INFO 4
#define SYS_PLATFORM_GET_RPMB_INFO 5
#define SYS_PLATFORM_GET_UFS_INFO 6
#define SYS_PLATFORM_SET_UFS_INFO 7

// Scheduling of the calling thread and of guest vCPUs.
#define SYS_PLATFORM_SET_CUR_THREAD_AFFINITY 16
#define SYS_PLATFORM_SET_CUR_THREAD_REALTIME 17
#define SYS_PLATFORM_GET_CUR_THREAD_REALTIME 18
#define SYS_PLATFORM_SET_SCHED_PARAM 19
#define SYS_PLATFORM_GET_SCHED_PARAM 20
#define SYS_PLATFORM_SET_SCHED_ID 21
#define SYS_PLATFORM_SET_BINDLK_PCPU 22
#define SYS_PLATFORM_SET_VCPU_TIMESLICE 23
#define SYS_PLATFORM_GET_VCPU_TIMESLICE 24
#define SYS_PLATFORM_CONFIG_VCPU_GROUP 25
#define SYS_PLATFORM_REG_SCHED_SHM 26
#define SYS_PLATFORM_REG_SCHED_IRQ 27

// Guest crash dumps, tracing, interrupts and the watchdog.
#define SYS_PLATFORM_CONFIG_VCPU_DUMP 32
#define SYS_PLATFORM_CVMD_CMD 33
#define SYS_PLATFORM_REG_CVMD_SHM 34
#define SYS_PLATFORM_REG_CVMD_IRQ 35
#define SYS_PLATFORM_START_KTRACE 36
#define SYS_PLATFORM_KTRACE_CTRL 37
#define SYS_PLATFORM_IRQ_MONITOR_CONFIG 38
#define SYS_PLATFORM_START_SMMU 39
#define SYS_PLATFORM_TEST_WDT 40

// Requests for NEBULA_GENERIC_FD.
#define SYS_PLATFORM_UART_ACTION_PRINT_LOG_OFF_CMD 48
