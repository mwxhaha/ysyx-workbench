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
                { top->pc_in = 0x10000001; top->pc_en = 1; });
        assert(top->pc_out == 0x10000001);
        set_pin([&]
                { top->pc_in = 0x00000000; top->pc_en = 1; });
        assert(top->pc_out == 0x00000000);
        set_pin([&]
                { top->pc_in = 0x10000001; top->pc_en = 0; });
        assert(top->pc_out == 0x00000000);
        set_pin([&]
                { top->pc_in = 0x11111111; top->pc_en = 1; });
        assert(top->pc_out == 0x11111111);
        set_pin([&]
                { top->pc_in = 0x00000000; top->pc_en = 0; });
        assert(top->pc_out == 0x11111111);
    }
#endif
}
