#include <stdio.h>
#include <assert.h>
#include "shift_register_sim.hpp"
#include "verilated.h"
#include "Vtop.h"
#include "verilated_vcd_c.h"
#include "sim_tool.hpp"

extern VerilatedContext *contextp;
extern Vtop *top;
extern VerilatedVcdC *tfp;

void shift_register_sim()
{
#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        update();
    }
#else
    unsigned int sim_time = 100;
    reset(5);
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        top->func = 0b001;
        top->q_parallel_in = 0b1001;
        cycle(1);
        top->func = 0b010;
        cycle(1);
        top->func = 0b011;
        cycle(1);
        top->func = 0b100;
        cycle(1);
        top->func = 0b101;
        top->q_serial_in = 0b0;
        cycle(1);
        cycle(1);
        top->func = 0b110;
        cycle(1);
        top->func = 0b111;
        cycle(1);
        top->func = 0b000;
        cycle(1);
    }
#endif
}
