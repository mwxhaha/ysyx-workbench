#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "nvboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #define V_TOP_NAME Vtop

void nvboard_bind_all_pins(V_TOP_NAME* top);

int main(int argc, char **argv)
{
    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    V_TOP_NAME *top = new V_TOP_NAME{contextp};

    Verilated::traceEverOn(true); 
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99); 
    tfp->open("simx.vcd");

    nvboard_bind_all_pins(top);
    nvboard_init();

    while (!contextp->gotFinish())
    {
        contextp->timeInc(1);
        top->eval();
        assert(top->f == (top->a ^ top->b));
        tfp->dump(contextp->time());
        nvboard_update();
        printf("time = %ld, a = %d, b = %d, f = %d\n", contextp->time(), top->a, top->b, top->f);
    }

    top->final();
    delete top;
    delete contextp;

    tfp->close();
    delete tfp;

    nvboard_quit();

    return 0;
}
