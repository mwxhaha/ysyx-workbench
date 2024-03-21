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
    return addr_ifetch(addr);
}

int addr_read_dpic(int addr)
{
#if ISA_WIDTH == 64
    panic("memory read do not support ISA64");
#endif
    return addr_read(addr);
}

void addr_write_dpic(int addr, char mask, int data)
{
#if ISA_WIDTH == 64
    panic("memory write do not support ISA64");
#endif
    addr_write(addr, mask, data);
}
