#ifndef CPU_HPP
#define CPU_HPP

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <Vtop.h>
#include <Vtop___024root.h>

#define CONFIG_RV32I 0
#define CONFIG_RV32E 1
#define CONFIG_RV64I 2

#if CONFIG_ISA == CONFIG_RV32I
#define ISA_WIDTH 32
#define GPR_NUM 32
#define INST_LEN 32
#elif CONFIG_ISA == CONFIG_RV32E
#define ISA_WIDTH 32
#define GPR_NUM 16
#define INST_LEN 32
#elif CONFIG_ISA == CONFIG_RV64I
#define ISA_WIDTH 64
#define GPR_NUM 32
#define INST_LEN 32
#else
#error "do not support ISA " #CONFIG_ISA
#endif

#if ISA_WIDTH == 32
typedef uint32_t word_t;
typedef int32_t sword_t;
#define FMT_WORD "0x%08x"
#define FMT_WORD_T "%u"
#define FMT_SWORD_T "%d"
#elif ISA_WIDTH == 64
typedef uint64_t word_t;
typedef int64_t sword_t;
#define FMT_WORD "0x%016lx"
#define FMT_WORD_T "%lu"
#define FMT_SWORD_T "%ld"
#else
#error "do not support ISA_WIDTH " #ISA_WIDTH
#endif

#if INST_LEN == 32
typedef uint32_t inst_t;
#define FMT_INST "0x%08x"
#else
#error "do not support INST_LEN " #INST_LEN
#endif

typedef word_t vaddr_t;
typedef word_t paddr_t;
#define FMT_PADDR FMT_WORD

#define CYCLE 2
#ifdef SIM_ALL
#define TOP_INST top->rootp->ysyx_23060075_cpu__DOT__mem_1_r
#define TOP_PC top->rootp->ysyx_23060075_cpu__DOT__core_1__DOT__pc
#define TOP_GPR top->rootp->ysyx_23060075_cpu__DOT__core_1__DOT__idu_1__DOT__gpr_1__DOT__reg_file_gpr__DOT__rf
#else
#define TOP_INST 0
#define TOP_PC 0
#define TOP_GPR ((word_t *)0)
#endif

enum
{
    NPC_RUNNING,
    NPC_STOP,
    NPC_END,
    NPC_ABORT,
    NPC_QUIT
};

typedef struct npc_state_t
{
    int state;
    vaddr_t halt_pc;
    word_t halt_ret;
} npc_state_t;
extern npc_state_t npc_state;

typedef struct
{
    word_t gpr[GPR_NUM];
    vaddr_t pc;
} CPU_state;

void sim();

#endif
