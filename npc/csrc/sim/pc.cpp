#ifdef SIM_pc
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <util/log.hpp>
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
#endif
