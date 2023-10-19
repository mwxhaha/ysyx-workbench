#include <sim_tool.hpp>
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
uint8_t mem[MEM_MAX] = {0xb3, 0x8c, 0x19, 0x01,
                        0x93, 0x89, 0x18, 0x80,
                        0xa3, 0xa8, 0x3c, 0x83,
                        0xe3, 0x88, 0x99, 0x83,
                        0x97, 0x18, 0x00, 0x80,
                        0xef, 0x08, 0x40, 0x00,
                        0x73, 0x00, 0x10, 0x80};
npc_state_t npc_state = {0, npc_running, MEM_BASE_ADDR};

void sim_init(int &argc, char **argv)
{
    init(argc, argv);
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

void absort_dpic(int pc)
{
    npc_state.ret = 1;
    npc_state.state = npc_absort;
    npc_state.pc = pc;
}

void ebreak_dpic(int ret, int pc)
{
    npc_state.ret = ret;
    npc_state.state = npc_end;
    npc_state.pc = pc;
}

void pmem_read(int raddr, int *rdata)
{
    assert((vaddr_t)raddr >= MEM_BASE_ADDR);
    assert((vaddr_t)raddr <= MEM_BASE_ADDR + MEM_MAX - 1);
    void *addr_real = (vaddr_t)raddr - MEM_BASE_ADDR + mem;
    *rdata = *(word_t *)addr_real;
}

void pmem_write(int waddr, int wdata, char wmask)
{
    assert((vaddr_t)waddr >= MEM_BASE_ADDR);
    assert((vaddr_t)waddr <= MEM_BASE_ADDR + MEM_MAX - 1);
    void *addr_real = (vaddr_t)waddr - MEM_BASE_ADDR + mem;
    switch (wmask)
    {
    case 1:
        *(uint8_t *)addr_real = wdata;
        break;
    case 3:
        *(uint16_t *)addr_real = wdata;
        break;
    case 7:
        *(uint32_t *)addr_real = wdata;
        break;
    default:
        assert(0);
        break;
    }
}
