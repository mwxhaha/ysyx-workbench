#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#include <sim_tool.hpp>
#include <iostream>
#include <cstdint>
#include <cassert>
#include <sdb.hpp>

void sim()
{
#ifdef NV_SIM
    assert(0);
#else
    sdb_mainloop();
    // int sim_time = 100000;
    // reset();
    // while (contextp->time() < sim_time && !contextp->gotFinish() && npc_state.state == run)
    // {
    //     cycle();
    // }
#endif
}

