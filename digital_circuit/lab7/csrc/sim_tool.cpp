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

void update(int time)
{
    while (time > 0)
    {
        top->eval();
#ifdef NV_SIM
        nvboard_update();
#else
        tfp->dump(contextp->time());
#endif
        contextp->timeInc(1);
        time--;
    }
}

void cycle(int cycle_number, int cycle_time)
{
    while (cycle_number > 0)
    {
        top->clk = 1;
        update(cycle_time / 2);
        top->clk = 0;
        update(cycle_time / 2);
        cycle_number--;
    }
}

void reset(int reset_cycle_number, int cycle_time)
{
    set_pin([&]
            { top->rst = 1; },
            cycle_time);
    cycle(reset_cycle_number - 1, cycle_time);
    set_pin([&]
            { top->rst = 0; },
            cycle_time);
}
