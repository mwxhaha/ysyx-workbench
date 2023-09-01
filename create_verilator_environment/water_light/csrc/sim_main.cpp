#include "Vlight.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "nvboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #define V_TOP_NAME Vlight

void nvboard_bind_all_pins(V_TOP_NAME* top);
VerilatedContext *contextp;
V_TOP_NAME *top;
VerilatedVcdC *tfp;

void update()
{
    top->eval();
    // tfp->dump(contextp->time());
    nvboard_update();
    contextp->timeInc(1);
}

void single_cycle()
{
    top->clk = 0;
    update();
    top->clk = 1;
    update();
}

void reset(int n)
{
    top->rst = 1;
    while (n-- > 0)
        single_cycle();
    top->rst = 0;
}

int main(int argc, char **argv)
{
    contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    top = new V_TOP_NAME{contextp};

    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, 10);
    tfp->open("build/light.vcd");

    nvboard_bind_all_pins(top);
    nvboard_init();

    // unsigned int sim_time = 100000;
    reset(10);
    while (!contextp->gotFinish())
    {
        single_cycle();
    }

    top->final();
    delete top;
    delete contextp;

    tfp->close();
    delete tfp;

    nvboard_quit();

    return 0;
}
