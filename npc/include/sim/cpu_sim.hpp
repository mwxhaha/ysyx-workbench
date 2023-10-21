#ifndef CPU_SIM_HPP
#define CPU_SIM_HPP

#include <cstdint>

#define CONFIG_RV32I 0
#define CONFIG_RV32E 1
#define CONFIG_RV64I 2

#if CONFIG_ISA == CONFIG_RV32I
#define ISA_WIDTH 32
using word_t = uint32_t;
using sword_t = int32_t;
#define FMT_WORD "0x%08x"
#define FMT_WORD_T "%u"
#define FMT_SWORD_T "%d"
#define GPR_NUM 32
#define INST_LEN 32
using inst_t = uint32_t;
#define FMT_INST "0x%08x"
using vaddr_t = word_t;
#elif CONFIG_ISA == CONFIG_RV32E
#define ISA_WIDTH 32
using word_t = uint32_t;
using sword_t = int32_t;
#define FMT_WORD "0x%08x"
#define FMT_WORD_T "%u"
#define FMT_SWORD_T "%d"
#define GPR_NUM 16
#define INST_LEN 32
using inst_t = uint32_t;
#define FMT_INST "0x%08x"
using vaddr_t = word_t;
#elif CONFIG_ISA == CONFIG_RV64I
#define ISA_WIDTH 64
using word_t = uint64_t;
using sword_t = int64_t;
#define FMT_WORD "0x%016lx"
#define FMT_WORD_T "%lu"
#define FMT_SWORD_T "%ld"
#define GPR_NUM 32
#define INST_LEN 32
using inst_t = uint32_t;
#define FMT_INST "0x%08x"
using vaddr_t = word_t;
#else
#error "do not support ISA " #CONFIG_ISA
#endif

#define MEM_BASE_ADDR 0x80000000
#define MEM_MAX 0x8000000

extern uint8_t mem[MEM_MAX];
enum state_t
{
    npc_running,
    npc_stop,
    npc_end,
    npc_absort,
    npc_quit
};
typedef struct npc_state_t
{
    word_t ret;
    state_t state;
    vaddr_t pc;
} npc_state_t;
extern npc_state_t npc_state;

typedef struct
{
    word_t gpr[GPR_NUM];
    vaddr_t pc;
} CPU_state;

#endif
