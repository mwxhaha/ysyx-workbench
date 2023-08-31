#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char **argv)
{
    VerilatedContext *contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    Vtop *top = new Vtop{contextp};

    Verilated::traceEverOn(true); 
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99); 
    tfp->open("result.vcd");

    int sim_time = 100;
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        int a = rand() & 1;
        int b = rand() & 1;
        top->a = a;
        top->b = b;

        contextp->timeInc(1);
        top->eval();
        tfp->dump(contextp->time());

        printf("time = %ld, a = %d, b = %d, f = %d\n", contextp->time(), a, b, top->f);
        assert(top->f == (a ^ b));
    }

    tfp->close();
    delete tfp;

    top->final();
    delete top;
    delete contextp;

    return 0;
}
