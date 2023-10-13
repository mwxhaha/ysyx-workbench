#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#include "sim_tool.hpp"
#include <iostream>
#include <cstdint>
#include <cassert>

void sim()
{
#ifdef NV_SIM
    assert(0);
#else
    int sim_time = 100000;
    reset();
    while (contextp->time() < sim_time && !contextp->gotFinish() && npc_state.state == run)
    {
        cycle();
    }
#endif
}
