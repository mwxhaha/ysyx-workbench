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
uint8_t mem[MEM_MAX] = {0xb3, 0x8c, 0x19, 0x01, 0x93, 0x89, 0x18, 0x80, 0xa3, 0xa8, 0x3c, 0x83, 0xe3, 0x88, 0x99, 0x83, 0x97, 0x18, 0x00, 0x80, 0xef, 0x18, 0x00, 0x80, 0x73, 0x00, 0x10, 0x80};

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
        top->mem_r_1 = memory_read(top->mem_r_1_addr, 4);
        top->mem_r_2 = memory_read(top->mem_r_2_addr, 4);
        top->mem_r_1 = memory_read(top->mem_r_1_addr, 4);
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

void memory_read(word_t addr, word_t data, int len)
{
    if (addr < MEM_BASE_ADDR)
        addr = MEM_BASE_ADDR;
    if (addr > MEM_BASE_ADDR + MEM_MAX - 1)
        addr = MEM_BASE_ADDR + MEM_MAX - 1;
    void *addr_real = mem + addr - MEM_BASE_ADDR;
    switch (len)
    {
    case 1:
        *(uint8_t *)addr_real = data;
        break;
    case 2:
        *(uint16_t *)addr_real = data;
        break;
    case 4:
        *(uint32_t *)addr_real = data;
        break;
    case 8:
        *(uint64_t *)addr_real = data;
        break;
    default:
        assert(0);
        break;
    }
}