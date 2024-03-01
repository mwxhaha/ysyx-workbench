/***************************************************************************************
 * Copyright (c) 2014-2022 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan
 *PSL v2. You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
 *KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 *NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <generated/autoconf.h>
#include <inttypes.h>
#include <macro.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef CONFIG_TARGET_AM
#include <klib.h>
#else
#include <assert.h>
#include <stdlib.h>
#endif

#if CONFIG_MBASE + CONFIG_MSIZE > 0x100000000ul
#define PMEM64 1
#endif

typedef MUXDEF(CONFIG_ISA64, uint64_t, uint32_t) word_t;
typedef MUXDEF(CONFIG_ISA64, int64_t, int32_t) sword_t;
#define FMT_WORD MUXDEF(CONFIG_ISA64, "0x%016" PRIx64, "0x%08" PRIx32)
#define FMT_WORD_T MUXDEF(CONFIG_ISA64, "%lu", "%u")
#define FMT_SWORD_T MUXDEF(CONFIG_ISA64, "%ld", "%d")

#ifdef CONFIG_ISA_riscv
#define GPR_NUM MUXDEF(CONFIG_RVE, 16, 32)

#define INST_LEN 32
typedef uint32_t inst_t;
#define FMT_INST "0x%08" PRIx32
#else
#error "not support this ISA"
#endif

typedef word_t vaddr_t;
// typedef MUXDEF(PMEM64, uint64_t, uint32_t) paddr_t;
typedef word_t paddr_t;
// #define FMT_PADDR MUXDEF(PMEM64, "0x%016" PRIx64, "0x%08" PRIx32)
#define FMT_PADDR MUXDEF(CONFIG_ISA64, "0x%016" PRIx64, "0x%08" PRIx32)
typedef uint16_t ioaddr_t;

#include <debug.h>

#endif
