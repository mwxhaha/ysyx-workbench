#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#include "sim_tool.hpp"

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
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        set_pin([&]
                { top->instruction = 0x00009117; });
        set_pin([&]
                { top->instruction = 0x00100073; });
    }
#endif
}
