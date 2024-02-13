#include <Vtop__Dpi.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <sim/cpu.hpp>
#include <util/log.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>
#include <cpu_exec/mem.hpp>

extern "C" void absort(int pc)
{
    npc_state.halt_ret = 1;
    npc_state.state = NPC_ABORT;
    npc_state.halt_pc = pc;
}

extern "C" void ebreak(int ret, int pc)
{
    npc_state.halt_ret = ret;
    npc_state.state = NPC_END;
    npc_state.halt_pc = pc;
}

extern "C" void disable_mtrace_once_1()
{
    disable_mtrace_once();
}

extern "C" void pmem_read(int addr, int *data)
{
#if ISA_WIDTH == 64
    panic("memory read do not support ISA64");
#endif
    *data = paddr_read(addr, 4);
}

extern "C" void pmem_write(int addr, char mask, int data)
{
#if ISA_WIDTH == 64
    panic("memory write do not support ISA64");
#endif
    int addr_shift;
    char mask_shift;
    int data_shift;
    if (((mask & 1) == 1))
    {
        addr_shift = addr;
        mask_shift = mask;
        data_shift = data;
    }
    else if ((mask & 2) == 2)
    {
        addr_shift = addr + 1;
        mask_shift = (unsigned)mask >> 1;
        data_shift = (unsigned)data >> 8;
    }
    else if ((mask & 4) == 4)
    {
        addr_shift = addr + 2;
        mask_shift = (unsigned)mask >> 2;
        data_shift = (unsigned)data >> 16;
    }
    else if ((mask & 8) == 8)
    {
        addr_shift = addr + 3;
        mask_shift = (unsigned)mask >> 3;
        data_shift = (unsigned)data >> 24;
    }
    else
    {
        panic("memory write mask 0x%x shift error at addr = " FMT_PADDR " at pc = " FMT_WORD, mask, addr, TOP_PC);
    }

    switch (mask_shift)
    {
    case 1:
        paddr_write(addr_shift, 1, data_shift);
        break;
    case 3:
        paddr_write(addr_shift, 2, data_shift);
        break;
    case 15:
        paddr_write(addr_shift, 4, data_shift);
        break;
    default:
        panic("memory write mask 0x%x change error at addr = " FMT_PADDR " at pc = " FMT_WORD, mask, addr, TOP_PC);
    }
}

