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
                { top->wen = 1;
        top->waddr = 0x00;
        top->wdata = 0x12131415; });
        set_pin([&]
                { 
        top->waddr = 0x01;
        top->wdata = 0x22232425; });
        set_pin([&]
                { 
        top->waddr = 0x1e;
        top->wdata = 0x32333435; });
        set_pin([&]
                { 
        top->waddr = 0x1f;
        top->wdata = 0x42434445; });
        set_pin([&]
                { 
        top->wen = 0;
        top->raddr_1 = 0x00;
        top->raddr_2 = 0x01; });
        set_pin([&]
                { 
        top->raddr_1 = 0x1e;
        top->raddr_2 = 0x1f; });
    }
#endif
}
