// #define NV_SIM
// #define V_TOP_NAME Vadder

#include "Vadder4.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#ifdef NV_SIM
#include "nvboard.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef NV_SIM
void nvboard_bind_all_pins(V_TOP_NAME *top);
#endif
VerilatedContext *contextp;
V_TOP_NAME *top;
VerilatedVcdC *tfp;

void update()
{
    top->eval();
    contextp->timeInc(1);
#ifdef NV_SIM
    nvboard_update();
#else
    tfp->dump(contextp->time());
#endif
}

// void single_cycle()
// {
//     top->clk = 0;
//     update();
//     top->clk = 1;
//     update();
// }

// void reset(int n)
// {
//     top->rst = 1;
//     while (n-- > 0)
//         single_cycle();
//     top->rst = 0;
// }

void sim_init(int argc, char **argv)
{
    contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    top = new V_TOP_NAME{contextp};

    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, 10);
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

int main(int argc, char **argv)
{
    sim_init(argc, argv);

#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        update();
    }
#else
    unsigned int sim_time = 10;
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        top->a = contextp->time();
        top->b = contextp->time() + 2;
        update();
        printf("%d %d %d\n", top->a, top->b, top->s);
    }
#endif

    sim_exit();

    return 0;
}
