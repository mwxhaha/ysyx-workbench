#ifndef CPU_SIM_HPP
#define CPU_SIM_HPP

#include <cstdint>

#define CONFIG_RV32I 0
#define CONFIG_RV32E 1
#define CONFIG_RV64I 2

#if CONFIG_ISA == CONFIG_RV32I
using word_t = uint32_t;
using vaddr_t = word_t;
using inst_t = uint32_t;
#define ISA_WIDTH 32
#define GPR_NUM 32
#define INST_LEN 32
#define FMT_WORD "0x%08x"
#define FMT_WORD_T "%u"
#define FMT_SWORD_T "%d"
#elif CONFIG_ISA == CONFIG_RV32E
using word_t = uint32_t;
using vaddr_t = word_t;
using inst_t = uint32_t;
#define ISA_WIDTH 32
#define GPR_NUM 16
#define INST_LEN 32
#define FMT_WORD "0x%08x"
#define FMT_WORD_T "%u"
#define FMT_SWORD_T "%d"
#elif CONFIG_ISA == CONFIG_RV64I
using word_t = uint64_t;
using vaddr_t = word_t;
using inst_t = uint32_t;
#define ISA_WIDTH 64
#define GPR_NUM 32
#define INST_LEN 32
#define FMT_WORD "0x%016x"
#define FMT_WORD_T "%lu"
#define FMT_SWORD_T "%ld"
#else
#error "do not support ISA " #CONFIG_ISA
#endif

#define MEM_BASE_ADDR 0x80000000
#define MEM_MAX 1000000

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

#endif
