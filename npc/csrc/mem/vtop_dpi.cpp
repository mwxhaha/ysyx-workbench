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
#include <mem/vaddr.hpp>

void absort(int pc)
{
    npc_state.halt_ret = 1;
    npc_state.state = NPC_ABORT;
    npc_state.halt_pc = pc;
}

void ebreak(int ret, int pc)
{
    npc_state.halt_ret = ret;
    npc_state.state = NPC_END;
    npc_state.halt_pc = pc;
}

int addr_ifetch_dpic(int addr)
{
#if ISA_WIDTH == 64
    panic("memory read do not support ISA64");
#endif
    return addr_ifetch(addr, 4);
}

int addr_read_dpic(int addr)
{
#if ISA_WIDTH == 64
    panic("memory read do not support ISA64");
#endif
    if (!TOP_IS_LS)
        return addr_ifetch(addr, 4);
    word_t data_shift;
    switch (addr & 0x3)
    {
    case 0:
        return addr_read(addr, 4);
        break;
    case 1:
        data_shift = addr_read(addr, 1);
        return data_shift << 8;
        break;
    case 2:
        data_shift = addr_read(addr, 2);
        return data_shift << 16;
        break;
    case 3:
        data_shift = addr_read(addr, 1);
        return data_shift << 24;
        break;
    default:
        panic("memory read len error at addr = " FMT_PADDR " at pc = " FMT_WORD, addr, TOP_PC);
    }
}

void addr_write_dpic(int addr, char mask, int data)
{
#if ISA_WIDTH == 64
    panic("memory write do not support ISA64");
#endif

    char mask_shift;
    word_t data_shift;
    if (((mask & 0x1) == 1))
    {
        mask_shift = mask;
        data_shift = data;
    }
    else if ((mask & 0x2) == 2)
    {
        mask_shift = mask >> 1;
        data_shift = data >> 8;
    }
    else if ((mask & 0x4) == 4)
    {
        mask_shift = mask >> 2;
        data_shift = data >> 16;
    }
    else if ((mask & 0x8) == 8)
    {
        mask_shift = mask >> 3;
        data_shift = data >> 24;
    }
    else
    {
        panic("memory write mask 0x%x bus align error at addr = " FMT_PADDR " at pc = " FMT_WORD, mask, addr, TOP_PC);
    }

    switch (mask_shift)
    {
    case 0x1:
        addr_write(addr, 1, data_shift);
        break;
    case 0x3:
        addr_write(addr, 2, data_shift);
        break;
    case 0xf:
        addr_write(addr, 4, data_shift);
        break;
    default:
        panic("memory write mask 0x%x change to len error at addr = " FMT_PADDR " at pc = " FMT_WORD, mask, addr, TOP_PC);
    }
}
