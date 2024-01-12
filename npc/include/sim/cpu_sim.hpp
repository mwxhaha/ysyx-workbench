#ifndef CPU_SIM_HPP
#define CPU_SIM_HPP

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <util/debug.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>

#define CONFIG_RV32I 0
#define CONFIG_RV32E 1
#define CONFIG_RV64I 2

#if CONFIG_ISA == CONFIG_RV32I
#define ISA_WIDTH 32
typedef uint32_t word_t;
typedef int32_t sword_t;
#define FMT_WORD "0x%08x"
#define FMT_WORD_T "%u"
#define FMT_SWORD_T "%d"
#define GPR_NUM 32
#define INST_LEN 32
typedef uint32_t inst_t;
#define FMT_INST "0x%08x"
#elif CONFIG_ISA == CONFIG_RV32E
#define ISA_WIDTH 32
typedef uint32_t word_t;
typedef int32_t sword_t;
#define FMT_WORD "0x%08x"
#define FMT_WORD_T "%u"
#define FMT_SWORD_T "%d"
#define GPR_NUM 16
#define INST_LEN 32
typedef uint32_t inst_t;
#define FMT_INST "0x%08x"
#elif CONFIG_ISA == CONFIG_RV64I
#define ISA_WIDTH 64
typedef uint64_t word_t;
typedef int64_t sword_t;
#define FMT_WORD "0x%016lx"
#define FMT_WORD_T "%lu"
#define FMT_SWORD_T "%ld"
#define GPR_NUM 32
#define INST_LEN 32
typedef uint32_t inst_t;
#define FMT_INST "0x%08x"
#else
#error "do not support ISA " #CONFIG_ISA
#endif

typedef word_t vaddr_t;
typedef word_t paddr_t;
#define FMT_PADDR FMT_WORD

#define CYCLE 2

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
