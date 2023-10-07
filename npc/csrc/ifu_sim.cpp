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
                { top->mem_r = 0x01198cb3; });
        set_pin([&]
                { top->mem_r = 0x80188993; });
        set_pin([&]
                { top->mem_r = 0x833ca8a3; });
        set_pin([&]
                { top->mem_r = 0x839988e3; });
        set_pin([&]
                { top->mem_r = 0x80001897; });
        set_pin([&]
                { top->mem_r = 0x800018ef; });
        set_pin([&]
                { top->mem_r = 0x00100073; });
    }
#endif
}
