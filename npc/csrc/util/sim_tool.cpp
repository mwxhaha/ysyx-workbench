#include <util/sim_tool.hpp>

#include <iostream>

#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#ifdef NV_SIM
#include <nvboard.h>
void nvboard_bind_all_pins(Vtop *top);
#endif
#include <cpu_exec/cpu_exec.hpp>
#include <monitor/monitor.hpp>

VerilatedContext *contextp;
Vtop *top;
VerilatedVcdC *tfp;
#define MAX_RECORD_WAVE 1000
#define HIERARCHY_DEEP 100

void sim_init(int argc, char *argv[])
{

    contextp = new VerilatedContext;
#ifdef SIM_ALL
    init_monitor(argc, argv);
#else
    contextp->commandArgs(argc, argv);
#endif
    top = new Vtop{contextp};

#ifdef CONFIG_RECORD_WAVE
    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, HIERARCHY_DEEP);
    tfp->open("build/wave.vcd");
#endif

#ifdef NV_SIM
    nvboard_bind_all_pins(top);
    nvboard_init();
#endif

#ifdef HAVE_CLK
    top->clk = 1;
    top->rst = 1;
#endif
    update(1);
}

void sim_exit()
{
    top->final();
    delete top;
    delete contextp;

#ifdef CONFIG_RECORD_WAVE
    tfp->close();
    delete tfp;
#endif

#ifdef NV_SIM
    nvboard_quit();
#endif

#ifdef SIM_ALL
    monitor_quit();
#endif
}

static uint64_t last_record_time = 0;

void update(int time)
{
    while (time > 0)
    {
        top->eval();
#ifdef NV_SIM
        nvboard_update();
#endif
#ifdef CONFIG_RECORD_WAVE
        if (g_nr_guest_inst >= 0 && g_nr_guest_inst <= MAX_RECORD_WAVE)
        {
            tfp->dump(contextp->time());
        }
#endif
        contextp->timeInc(1);
        time--;
    }
}

void cycle(int cycle_number, int cycle_time)
{
    while (cycle_number > 0)
    {
#ifdef HAVE_CLK
        top->clk = 1;
#endif
        update(cycle_time / 2 - 1);
#ifdef HAVE_CLK
        top->clk = 0;
#endif
        update(cycle_time / 2);
#ifdef HAVE_CLK
        top->clk = 1;
#endif
        update(1);
        cycle_number--;
    }
}

void reset(int reset_cycle_number, int cycle_time)
{
#ifdef HAVE_CLK
    top->rst = 1;
#endif
    cycle(reset_cycle_number, cycle_time);
    set_pin([&]
            {
#ifdef HAVE_CLK
                top->rst = 0;
#endif
            },
            cycle_time);
}
