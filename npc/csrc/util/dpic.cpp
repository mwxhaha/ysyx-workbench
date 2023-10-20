#include <Vtop__Dpi.h>

#include <cassert>

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

extern "C" void pmem_read(int raddr, int *rdata)
{
    assert((vaddr_t)raddr >= MEM_BASE_ADDR);
    assert((vaddr_t)raddr <= MEM_BASE_ADDR + MEM_MAX - 1);
    void *addr_real = (vaddr_t)raddr - MEM_BASE_ADDR + mem;
    *rdata = *(word_t *)addr_real;
}

extern "C" void pmem_write(int waddr, int wdata, char wmask)
{
    assert((vaddr_t)waddr >= MEM_BASE_ADDR);
    assert((vaddr_t)waddr <= MEM_BASE_ADDR + MEM_MAX - 1);
    void *addr_real = (vaddr_t)waddr - MEM_BASE_ADDR + mem;
    switch (wmask)
    {
    case 1:
        *(uint8_t *)addr_real = wdata;
        break;
    case 3:
        *(uint16_t *)addr_real = wdata;
        break;
    case 7:
        *(uint32_t *)addr_real = wdata;
        break;
    default:
        assert(0);
        break;
    }
}
