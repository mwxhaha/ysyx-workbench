#include <util/sim_tool.hpp>
#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#ifdef NV_SIM
#include <nvboard.h>
void nvboard_bind_all_pins(Vtop *top);
#endif
#include <cassert>
#include <cstdint>
#include <cstring>
#include <Vtop__Dpi.h>
#include <cpu_init.hpp>

VerilatedContext *contextp;
Vtop *top;
VerilatedVcdC *tfp;
#define MAX_TRACE_TIME 100000
#define HIERARCHY_DEEP 100

void sim_init(int &argc, char **argv)
{
#ifdef SIM_ALL
    init(argc, argv);
#endif
    contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    top = new Vtop{contextp};

#ifdef CONFIG_WTRACE
    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, HIERARCHY_DEEP);
    tfp->open("build/wave.vcd");
#endif

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

#ifdef CONFIG_WTRACE
    tfp->close();
    delete tfp;
#endif

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
#endif
#ifdef CONFIG_WTRACE
        if (contextp->time() < MAX_TRACE_TIME)
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
    top->rst = 1;
    cycle(reset_cycle_number, cycle_time);
    set_pin([&]
            { top->rst = 0; },
            cycle_time);
}
