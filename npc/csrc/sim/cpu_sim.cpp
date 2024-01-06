#include <sim/cpu_sim.hpp>

#include <util/sim_tool.hpp>
#include <monitor/cpu_sdb.hpp>
#include <cpu/cpu_mem.hpp>
#include <util/debug.hpp>

npc_state_t npc_state = {1, NPC_STOP, MEM_BASE_ADDR};

void sim()
{
#ifdef NV_SIM
    panic("do not support nvboard");
#else
    reset();
    sdb_mainloop();
#endif
}
