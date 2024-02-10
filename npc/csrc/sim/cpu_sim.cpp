#include <sim/cpu_sim.hpp>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <util/debug.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>
#include <cpu/cpu_mem.hpp>
#include <monitor/cpu_sdb.hpp>

npc_state_t npc_state = {1, NPC_STOP, MEM_BASE_ADDR};

void sim()
{
#ifdef NV_SIM
    panic("do not support nvboard");
#else
    reset(3, CYCLE);
    sdb_mainloop();
#endif
}
