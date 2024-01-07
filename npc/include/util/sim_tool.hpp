#ifndef SIM_TOOL_HPP
#define SIM_TOOL_HPP

#include <iostream>
#include <bitset>

#include <verilated.h>
#include <verilated_vcd_c.h>

#include <Vtop.h>

extern VerilatedContext *contextp;
extern Vtop *top;
extern VerilatedVcdC *tfp;

void sim_init(int argc, char *argv[]);
void sim_exit();
void update(int time = 1);
void cycle(int cycle_number = 1, int cycle_time = 2);
void set_pin(auto f, int cycle_time = 2)
{
#ifdef HAVE_CLK
    top->clk = 1;
#endif
    update(1);
    f();
    update(cycle_time / 2 - 1);
#ifdef HAVE_CLK
    top->clk = 0;
#endif
    update(cycle_time / 2);
}
void reset(int reset_cycle_number = 3, int cycle_time = 2);
void pin_output(auto pin, int data_len, bool binary_mode, bool hex_mode, bool unsigned_mode, bool signed_mode)
{
    if (binary_mode)
        std::cout << std::bitset<sizeof(pin) * 8>(pin) << ' ';
    if (hex_mode)
        std::cout << std::hex << static_cast<unsigned long long>(pin) << std::dec << ' ';
    if (unsigned_mode)
        std::cout << static_cast<unsigned long long>(pin) << ' ';
    if (signed_mode)
        std::cout << pin - std::pow(2, data_len) << ' ';
    std::cout << std::endl;
}

#endif
