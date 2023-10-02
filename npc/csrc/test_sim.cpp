#include "test_sim.hpp"
#include <iostream>
#include <cassert>
#include "sim_tool.hpp"

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
