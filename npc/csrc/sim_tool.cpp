#include "sim_tool.hpp"
#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#ifdef NV_SIM
#include <nvboard.h>
void nvboard_bind_all_pins(Vtop *top);
#endif
#include <cassert>
#include <cstdint>

VerilatedContext *contextp;
Vtop *top;
VerilatedVcdC *tfp;
uint8_t mem[MEM_MAX] = {0x10, 0x11, 0x12, 0x13, 0x20, 0x21, 0x22, 0x23, 0x30, 0x31, 0x32, 0x33, 0x40, 0x41, 0x42, 0x43, 0x50, 0x51, 0x52, 0x53};

void sim_init(int argc, char **argv)
{
    contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    top = new Vtop{contextp};

    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, HIERARCHY_DEEP);
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
        // top->pc_mem_read = memory_read(top->pc_out, 4);
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

word_t memory_read(word_t addr, int len)
{
    if (addr < MEM_BASE_ADDR)
        addr = MEM_BASE_ADDR;
    if (addr > MEM_BASE_ADDR + MEM_MAX - 1)
        addr = MEM_BASE_ADDR + MEM_MAX - 1;
    void *addr_real = mem + addr - MEM_BASE_ADDR;
    switch (len)
    {
    case 1:
        return *(uint8_t *)addr_real;
        break;
    case 2:
        return *(uint16_t *)addr_real;
        break;
    case 4:
        return *(uint32_t *)addr_real;
        break;
    case 8:
        return *(uint64_t *)addr_real;
        break;
    default:
        assert(0);
        break;
    }
}
