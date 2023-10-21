#include <Vtop__Dpi.h>

#include <cassert>
#include <cstdio>

#include <verilated.h>
#include <util/sim_tool.hpp>
#include <sim/cpu_sim.hpp>

#ifdef SIM_ALL
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

static unsigned long long last_time = 0;

extern "C" void pmem_read(int raddr, int *rdata, int is_fetch)
{
#if (ISA_WIDTH == 64)
    assert(0)
#else
    assert((vaddr_t)raddr >= MEM_BASE_ADDR);
    assert((vaddr_t)raddr <= MEM_BASE_ADDR + MEM_MAX - 1);
    void *addr_real = (vaddr_t)raddr - MEM_BASE_ADDR + mem;
    *rdata = *(word_t *)addr_real;
#ifdef CONFIG_MTRACE
    if (contextp->time() % CYCLE == CYCLE - 1 && contextp->time() != last_time && !is_fetch)
    {
        last_time = contextp->time();
        printf("memory read in addr " FMT_WORD ": " FMT_WORD "\n", raddr, *rdata);
    }
#endif
#endif
}

extern "C" void pmem_write(int waddr, int wdata, char wmask)
{
#if (ISA_WIDTH == 64)
    assert(0)
#else
    assert((vaddr_t)waddr >= MEM_BASE_ADDR);
    assert((vaddr_t)waddr <= MEM_BASE_ADDR + MEM_MAX - 1);
    void *addr_real = (vaddr_t)waddr - MEM_BASE_ADDR + mem;
#ifdef CONFIG_MTRACE
    int write_data_before = *(uint32_t *)addr_real;
#endif
    switch (wmask)
    {
    case 1:
        *(uint8_t *)addr_real = wdata;
        break;
    case 3:
        *(uint16_t *)addr_real = wdata;
        break;
    case 15:
        *(uint32_t *)addr_real = wdata;
        break;
    default:
        assert(0);
        break;
    }
#ifdef CONFIG_MTRACE
    int write_data_after = *(uint32_t *)addr_real;
    printf("memory write in addr " FMT_WORD " with mask 0x%04x: " FMT_WORD "->" FMT_WORD "\n", waddr, wmask, write_data_before, write_data_after);
#endif
#endif
}

#else

extern "C" void absort_dpic(int pc) {}
extern "C" void ebreak_dpic(int ret, int pc) {}
extern "C" void pmem_read(int raddr, int *rdata) {}
extern "C" void pmem_write(int waddr, int wdata, char wmask) {}

#endif
