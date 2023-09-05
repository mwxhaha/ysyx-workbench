// #define NV_SIM
// #define V_TOP_NAME Vm_mux21

#include "Vmux21b.h"
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
#else
    unsigned int sim_time = 100;
    while (contextp->time() < sim_time && !contextp->gotFinish())
#endif
    {
        top->s=0; top->a=0; top->b=0;   update();   // 将s，a和b均初始化为“0”
                  top->b=1;             update();   // 将b改为“1”，s和a的值不变，继续保持“0”，
        top->a=1; top->b=0;             update();   // 将a，b分别改为“1”和“0”，s的值不变，
                  top->b=1;             update();   // 将b改为“1”，s和a的值不变，维持10个时间单位
        top->s=1; top->a=0; top->b=0;   update();   // 将s，a，b分别变为“1,0,0”，维持10个时间单位
                  top->b=1;             update();
        top->a=1; top->b=0;             update();
                  top->b=1;             update();
    }

    sim_exit();

    return 0;
}
