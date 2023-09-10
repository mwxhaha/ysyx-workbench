#ifndef sim_tool_hpp
#define sim_tool_hpp

#include <iostream>
#include <cmath>
#include <bitset>
#include "Vtop.h"

extern Vtop *top;

void sim_init(int argc, char **argv);
void sim_exit();
void update(int time = 1);
void cycle(int cycle_number, int cycle_time = 10);
void set_pin(auto f, int cycle_time = 10)
{
    top->clk = 1;
    update();
    f();
    update(cycle_time / 2 - 1);
    top->clk = 0;
    update(cycle_time / 2);
}
void reset(int reset_cycle_number, int cycle_time = 10);
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
}

#endif
