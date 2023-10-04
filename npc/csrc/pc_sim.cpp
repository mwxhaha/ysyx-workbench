#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#include "sim_tool.hpp"
#include <iostream>
#include <cassert>

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
                { top->din = 1; top->wen = 1; });
        set_pin([&]
                { top->din = 0; top->wen = 1; });
        set_pin([&]
                { top->din = 1; top->wen = 0; });
        set_pin([&]
                { top->din = 1; top->wen = 1; });
        set_pin([&]
                { top->din = 0; top->wen = 0; });
    }
#endif
}
