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
                { top->gpr_w_en = 1;
        top->gpr_w_addr = 0x00;
        top->gpr_w = 0x12131415; });
        set_pin([&]
                { 
        top->gpr_w_addr = 0x01;
        top->gpr_w = 0x22232425; });
        set_pin([&]
                { 
        top->gpr_w_addr = 0x1e;
        top->gpr_w = 0x32333435; });
        set_pin([&]
                { 
        top->gpr_w_addr = 0x1f;
        top->gpr_w = 0x42434445; });
        set_pin([&]
                { 
        top->gpr_w_en = 0;
        top->gpr_r_1_addr = 0x00;
        top->gpr_r_2_addr = 0x01; });
        set_pin([&]
                { 
        top->gpr_r_1_addr = 0x1e;
        top->gpr_r_2_addr = 0x1f; });
    }
#endif
}
