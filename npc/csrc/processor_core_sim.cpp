#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#include "sim_tool.hpp"
#include <iostream>
#include <cstdint>
#include <cassert>
#include <Vtop__Dpi.h>

void pmem_read(int raddr, int *rdata)
{
    assert((vaddr_t)raddr >= MEM_BASE_ADDR);
    assert((vaddr_t)raddr <= MEM_BASE_ADDR + MEM_MAX - 1);
    // if ((vaddr_t)raddr < MEM_BASE_ADDR)
    //     raddr = MEM_BASE_ADDR;
    // if ((vaddr_t)raddr > MEM_BASE_ADDR + MEM_MAX - 1)
    //     raddr = MEM_BASE_ADDR + MEM_MAX - 1;
    void *addr_real = (vaddr_t)raddr - MEM_BASE_ADDR + mem;
    *rdata = *(word_t *)addr_real;
}

void pmem_write(int waddr, int wdata, char wmask)
{
    assert((vaddr_t)waddr >= MEM_BASE_ADDR);
    assert((vaddr_t)waddr <= MEM_BASE_ADDR + MEM_MAX - 1);
    // if ((vaddr_t)waddr < MEM_BASE_ADDR)
    //     waddr = MEM_BASE_ADDR;
    // if ((vaddr_t)waddr > MEM_BASE_ADDR + MEM_MAX - 1)
    //     waddr = MEM_BASE_ADDR + MEM_MAX - 1;
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

void sim()
{
#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        update();
    }
#else
    int sim_time = 1000;
    reset();
    while (contextp->time() < sim_time && !contextp->gotFinish() && npc_state.state == run)
    {
        cycle();
    }
#endif
}
