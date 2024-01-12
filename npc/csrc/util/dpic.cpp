#include <Vtop__Dpi.h>

#ifdef SIM_ALL
#include <util/debug.hpp>
#include <util/sim_tool.hpp>
#include <cpu/cpu_mem.hpp>
#include <Vtop.h>

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

extern "C" void pmem_read_dpic(int addr, int *data)
{
    *data = pmem_read(addr, 4);
}

extern void assert_fail_msg();

extern "C" void pmem_write_dpic(int addr, char mask, int data)
{
    switch (mask)
    {
    case 1:
        pmem_write(addr, 1, data);
        break;
    case 3:
        pmem_write(addr, 2, data);
        break;
    case 15:
        pmem_write(addr, 4, data);
        break;
#if ISA_WIDTH == 64
    case 255:
        pmem_write(addr, 8, data);
        break;
#endif
    default:
        panic("memory write mask error");
    }
}

#else

extern "C" void absort_dpic(int pc) {}
extern "C" void ebreak_dpic(int ret, int pc) {}
extern "C" void disable_mtrace_once_dpic() {}
extern "C" void pmem_read_dpic(int addr, int *data) {}
extern "C" void pmem_write_dpic(int addr, char wask, int data) {}

#endif
