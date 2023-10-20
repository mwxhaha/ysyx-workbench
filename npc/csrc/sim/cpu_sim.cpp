#include <sim/cpu_sim.hpp>

#include <util/sim_tool.hpp>
#include <sdb/cpu_sdb.hpp>
#include <util/debug.hpp>


uint8_t mem[MEM_MAX] = {0xb3, 0x8c, 0x19, 0x01,
                        0x93, 0x89, 0x18, 0x80,
                        0xa3, 0xa8, 0x3c, 0x83,
                        0xe3, 0x88, 0x99, 0x83,
                        0x97, 0x18, 0x00, 0x80,
                        0xef, 0x08, 0x40, 0x00,
                        0x73, 0x00, 0x10, 0x80};
npc_state_t npc_state = {1, npc_stop, MEM_BASE_ADDR};

void sim()
{
#ifdef NV_SIM
    panic("do not support nvboard");
#else
    reset();
    sdb_mainloop();
    // int sim_time = 100000;
    // while (contextp->time() < sim_time && !contextp->gotFinish() && npc_state.state == npc_running)
    // {
    //     cycle();
    // }
#endif
}

