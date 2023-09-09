#include <stdio.h>
#include <assert.h>
#include "random_sim.hpp"
#include "verilated.h"
#include "Vtop.h"
#include "verilated_vcd_c.h"
#include "sim_tool.hpp"

extern VerilatedContext *contextp;
extern Vtop *top;
extern VerilatedVcdC *tfp;

void random_sim()
{
#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        update();
    }
#else
    unsigned int sim_time = 1000;
    reset(5);
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        top->init = 0b1;
        top->q_parallel_in = 0b11111111;
        cycle(1);
        top->init = 0b0;
        cycle(100);
    }
#endif
}
