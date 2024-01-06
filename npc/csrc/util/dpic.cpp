#include <Vtop__Dpi.h>

#ifdef SIM_ALL
#include <cpu/cpu_mem.hpp>
#include <sim/cpu_sim.hpp>

extern "C" void absort_dpic(int pc)
{
    npc_state.ret = 1;
    npc_state.state = npc_absort;
    npc_state.pc = pc;
}

extern "C" void ebreak_dpic(int ret, int pc)
{
    npc_state.ret = ret;
    npc_state.state = npc_end;
    npc_state.pc = pc;
}

extern "C" void mem_read(int raddr, int *rdata)
{
    pmem_read(raddr, (word_t *)rdata);
}

extern "C" void mem_write(int waddr, int wdata, char wmask)
{
    pmem_write(waddr, wdata, wmask);
}

#else

extern "C" void absort_dpic(int pc) {}
extern "C" void ebreak_dpic(int ret, int pc) {}
extern "C" void mem_read(int raddr, int *rdata) {}
extern "C" void mem_write(int waddr, int wdata, char wmask) {}

#endif
