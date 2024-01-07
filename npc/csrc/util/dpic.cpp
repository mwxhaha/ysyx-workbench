#include <Vtop__Dpi.h>

#ifdef SIM_ALL
#include <sim/cpu_sim.hpp>
#include <cpu/cpu_mem.hpp>

extern "C" void absort_dpic(int pc)
{
    npc_state.halt_ret = 1;
    npc_state.state = NPC_ABORT;
    npc_state.halt_pc = pc;
}

extern "C" void ebreak_dpic(int ret, int pc)
{
    npc_state.halt_ret = ret;
    npc_state.state = NPC_END;
    npc_state.halt_pc = pc;
}

extern "C" void disable_mtrace_once_dpic()
{
    disable_mtrace_once();
}

extern "C" void pmem_read_dpic(int raddr, int *rdata)
{
    pmem_read(raddr, (word_t *)rdata);
}

extern "C" void pmem_write_dpic(int waddr, int wdata, char wmask)
{
    pmem_write(waddr, wdata, wmask);
}

#else

extern "C" void absort_dpic(int pc) {}
extern "C" void ebreak_dpic(int ret, int pc) {}
extern "C" void disable_mtrace_once_dpic() {}
extern "C" void pmem_read_dpic(int raddr, int *rdata) {}
extern "C" void pmem_write_dpic(int waddr, int wdata, char wmask) {}

#endif
