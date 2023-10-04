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
                {top->pc_out=0x80000000;
            top->inst_num = 2;
            top->inst_type = 5;
            top->imm = 1;
            top->src1 = 2;
            top->src2 = 3;
            top->mem_r = 4; });
        set_pin([&]
                {top->inst_num = 3;
            top->inst_type = 6; });
        set_pin([&]
                {top->inst_num = 5;
            top->inst_type = 4; });
        set_pin([&]
                {top->inst_num = 20;
            top->inst_type = 3; });
        set_pin([&]
                {top->inst_num = 21;
            top->inst_type = 2; });
        set_pin([&]
                {top->inst_num = 31;
            top->inst_type = 1; });
        set_pin([&]
                {top->inst_num = 42;
            top->inst_type = 2; });
    }
#endif
}
