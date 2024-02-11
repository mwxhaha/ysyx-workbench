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
#include <verilated.h>
#include <Vtop.h>

void sim()
{
#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        cycle();
    }
#else
    int sim_time = 100;
    reset();
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        set_pin([&]
                { top->in = 1; });
        assert(top->q == 1);
        set_pin([&]
                { top->in = 0; });
        assert(top->q == 0);
        set_pin([&]
                { top->in = 1; });
        assert(top->q == 1);
        cycle(3);
        assert(top->q == 1);
        set_pin([&]
                { top->in = 0; });
        assert(top->q == 0);
        cycle(3);
        assert(top->q == 0);
    }
#endif
}
