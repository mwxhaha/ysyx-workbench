#ifndef SIM_TOOL_HPP
#define SIM_TOOL_HPP

#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#include <iostream>
#include <cmath>
#include <bitset>

extern VerilatedContext *contextp;
extern Vtop *top;
extern VerilatedVcdC *tfp;

void sim_init(int &argc, char **argv);
void sim_exit();
void update(int time = 1);
void cycle(int cycle_number = 1, int cycle_time = 10);
void set_pin(auto f, int cycle_time = 10)
{
    top->clk = 1;
    update();
    f();
    update(cycle_time / 2 - 1);
    top->clk = 0;
    update(cycle_time / 2);
}
void reset(int reset_cycle_number = 3, int cycle_time = 10);
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

#ifdef SIM_ALL
#define Assert(cond, format, ...)               \
    do                                          \
    {                                           \
        if (!(cond))                            \
        {                                       \
            printf(format "\n", ##__VA_ARGS__); \
            extern void assert_fail_msg();      \
            assert_fail_msg();                  \
            assert(cond);                       \
        }                                       \
    } while (0)
#else
#define Assert(cond, format, ...)               \
    do                                          \
    {                                           \
        if (!(cond))                            \
        {                                       \
            printf(format "\n", ##__VA_ARGS__); \
            assert(cond);                       \
        }                                       \
    } while (0)
#endif

#define panic(format, ...) Assert(0, format, ##__VA_ARGS__)

#endif
