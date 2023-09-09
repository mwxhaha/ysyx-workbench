#include <stdio.h>
#include <assert.h>
#include "barrel_shifter_sim.hpp"
#include "verilated.h"
#include "Vtop.h"
#include "verilated_vcd_c.h"
#include "sim_tool.hpp"

extern VerilatedContext *contextp;
extern Vtop *top;
extern VerilatedVcdC *tfp;

void barrel_shifter_sim()
{
#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        update();
    }
#else
    unsigned int sim_time = 1000;
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        for (unsigned int i = 0; i <= 1; i++)
            for (unsigned int j = 0; j <= 1; j++)
                for (unsigned int k = 0; k <= 7; k++)
                {
                    top->din = 0b10100101;
                    top->shamt = k;
                    top->left_or_right = i;
                    top->algorism_or_logic = j;
                    update();
                    printf("%d", top->dout);
                }
    }
#endif
}
