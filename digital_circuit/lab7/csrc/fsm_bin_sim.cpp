#include <iostream>
#include <cassert>
#include "fsm_bin_sim.hpp"
#include "verilated.h"
#include "Vtop.h"
#include "verilated_vcd_c.h"
#include "sim_tool.hpp"

extern VerilatedContext *contextp;
extern Vtop *top;
extern VerilatedVcdC *tfp;

void fsm_bin_sim()
{
#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        update();
    }
#else
    int sim_time = 1000;
    reset(5);
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        set_pin([&]
                { top->in = 1; });
        set_pin([&]
                { top->in = 0; });
        cycle(3);
        set_pin([&]
                { top->in = 1; });
        cycle(4);
        set_pin([&]
                { top->in = 0; });
        cycle(5);
        pin_output(top->out, 1, 1, 1, 1, 1);
        std::cout << std::endl;
    }
#endif
}
