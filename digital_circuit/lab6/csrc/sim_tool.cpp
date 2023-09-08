#include <stdlib.h>
#include "sim_tool.hpp"
#include "verilated.h"
#include "Vtop.h"
#include "verilated_vcd_c.h"
#ifdef NV_SIM
#include "nvboard.h"
void nvboard_bind_all_pins(Vtop *top);
#endif

extern VerilatedContext *contextp;
extern Vtop *top;
extern VerilatedVcdC *tfp;

void update()
{
    top->eval();
#ifdef NV_SIM
    nvboard_update();
#else
    tfp->dump(contextp->time());
    contextp->timeInc(1);
#endif
}

void cycle(int n)
{
    while (n > 0)
    {
        top->clk = 1;
        update();
        top->clk = 0;
        update();
        n--;
    }
}

void reset(int n)
{
    top->rst = 1;
    cycle(n);
    top->rst = 0;
}

void sim_init(int argc, char **argv)
{
    contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    top = new Vtop{contextp};

    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, 100);
    tfp->open("build/wave.vcd");

#ifdef NV_SIM
    nvboard_bind_all_pins(top);
    nvboard_init();
#endif
}

void sim_exit()
{
    top->final();
    delete top;
    delete contextp;

    tfp->close();
    delete tfp;

#ifdef NV_SIM
    nvboard_quit();
#endif
}