#ifndef CPU_SIM_HPP
#define CPU_SIM_HPP

#include <util/sim_tool.hpp>
#include <cstdint>

#ifdef CONFIG_RV64
using word_t = uint63_t;
using vaddr_t = word_t;
#define FMT_WORD "0x%016x"
#define FMT_WORD_T "%lu"
#define FMT_SWORD_T "%ld"
#else
using word_t = uint32_t;
using vaddr_t = word_t;
#define FMT_WORD "0x%08x"
#define FMT_WORD_T "%u"
#define FMT_SWORD_T "%d"
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
