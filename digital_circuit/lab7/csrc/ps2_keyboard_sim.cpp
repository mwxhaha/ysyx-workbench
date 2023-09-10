#include <iostream>
#include <cassert>
#include "ps2_keyboard_sim.hpp"
#include "verilated.h"
#include "Vtop.h"
#include "verilated_vcd_c.h"
#include "sim_tool.hpp"

extern VerilatedContext *contextp;
extern Vtop *top;
extern VerilatedVcdC *tfp;

void ps2_cycle(int value)
{
    top->ps2_data = value;
    top->ps2_clk = 1;
    cycle(5);
    top->ps2_clk = 0;
    cycle(5);
}

void ps2_sent(int key)
{
    ps2_cycle(0);
    auto key_binary = std::bitset<8>(key);
    int odd_cnt = 0;
    for (int i = 0; i <= 7; i++)
    {
        ps2_cycle(key_binary[i]);
        if (key_binary[i] == 1)
            odd_cnt++;
    }
    if (odd_cnt % 2 == 1)
        ps2_cycle(0);
    else
        ps2_cycle(1);
    ps2_cycle(1);
}

void sim()
{
#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        update();
    }
#else
    int sim_time = 10000;
    reset(5);
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        ps2_sent(0x1c);
        ps2_sent(0xf0);
        ps2_sent(0x1c);

        ps2_sent(0x12);
        ps2_sent(0xf0);
        ps2_sent(0x12);

        ps2_sent(0x12);
        ps2_sent(0x1b);
        ps2_sent(0xf0);
        ps2_sent(0x1b);
        ps2_sent(0xf0);
        ps2_sent(0x12);
    }
#endif
}
