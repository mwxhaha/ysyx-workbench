#ifdef SIM_gpr
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
                { top->gpr_w_en = 1; top->gpr_w_addr = 0x0; top->gpr_w = 0x12131415; });
        set_pin([&]
                { top->gpr_w_en = 1; top->gpr_w_addr = 0x1; top->gpr_w = 0x22232425; });
        set_pin([&]
                { top->gpr_w_en = 1; top->gpr_w_addr = 0xf; top->gpr_w = 0x32333435; });
        set_pin([&]
                { top->gpr_w_en = 0; top->gpr_w_addr = 0xe; top->gpr_w = 0x42434445; });
        set_pin([&]
                { top->gpr_1_addr = 0x0; top->gpr_2_addr = 0x1; });
        assert(top->gpr_1_r == 0x00000000);
        assert(top->gpr_2_r == 0x22232425);
        set_pin([&]
                { top->gpr_1_addr = 0xe; top->gpr_2_addr = 0xf; });
        assert(top->gpr_1_r == 0x00000000);
        assert(top->gpr_2_r == 0x32333435);
    }
#endif
}
#endif
