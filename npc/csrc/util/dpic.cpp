#include <Vtop__Dpi.h>
#include <sim/cpu_sim.hpp>

void absort_dpic(int pc)
{
    npc_state.ret = 1;
    npc_state.state = npc_absort;
    npc_state.pc = pc;
}

void ebreak_dpic(int ret, int pc)
{
    npc_state.ret = ret;
    npc_state.state = npc_end;
    npc_state.pc = pc;
}

void pmem_read(int raddr, int *rdata)
{
    Assert((vaddr_t)raddr >= MEM_BASE_ADDR, "memory read out of bound: " FMT_WORD, raddr);
    Assert((vaddr_t)raddr <= MEM_BASE_ADDR + MEM_MAX - 1, "memory read out of bound: " FMT_WORD, raddr);
    void *addr_real = (vaddr_t)raddr - MEM_BASE_ADDR + mem;
    *rdata = *(word_t *)addr_real;
}

void pmem_write(int waddr, int wdata, char wmask)
{
    Assert((vaddr_t)waddr >= MEM_BASE_ADDR, "memory write out of bound: " FMT_WORD, waddr);
    Assert((vaddr_t)waddr <= MEM_BASE_ADDR + MEM_MAX - 1, "memory write out of bound: " FMT_WORD, waddr);
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
        panic("illegal wmask");
        break;
    }
}