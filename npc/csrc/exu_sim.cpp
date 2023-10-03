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
                {top->pc=0x80000000;
            top->inst_num = 2;
            top->inst_type = 5;
            top->imm = 1; });
        cycle();
        set_pin([&]
                {top->pc=0x80000000;
            top->inst_num = 42;
            top->inst_type = 2;
            top->imm = 0; });
        cycle();
    }
#endif
}
