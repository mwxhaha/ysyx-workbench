#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#include <sim_tool.hpp>
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
    int sim_time = 100;
    reset(5);
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        set_pin([&]
                { top->in = 1; });
        set_pin([&]
                { top->in = 0; });
        set_pin([&]
                { top->in = 1; });
        cycle(3);
        set_pin([&]
                { top->in = 0; });
        cycle(3);
    }
#endif
}
