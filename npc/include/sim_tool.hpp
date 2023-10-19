#ifndef SIM_TOOL_HPP
#define SIM_TOOL_HPP

#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#include <iostream>
#include <cmath>
#include <bitset>
#include <cstdint>

#ifdef CONFIG_RV64
using word_t = uint63_t;
using vaddr_t = word_t;
#define FMT_WORD "0x%016x"
#define FMT_WORD_T "%lu"
#define FMT_SWORD_T "%ld"
#else
using word_t = uint32_t;
using vaddr_t = word_t;
#define FMT_WORD "0x%08x"
#define FMT_WORD_T "%u"
#define FMT_SWORD_T "%d"
#endif

#define HIERARCHY_DEEP 100
#define MEM_BASE_ADDR 0x80000000
#define MEM_MAX 1000000

extern VerilatedContext *contextp;
extern Vtop *top;
extern VerilatedVcdC *tfp;
extern uint8_t mem[MEM_MAX];
enum state_t
{
    npc_running,
    npc_stop,
    npc_end,
    npc_absort,
    npc_quit
};
typedef struct npc_state_t
{
    word_t ret;
    state_t state;
    vaddr_t pc;
} npc_state_t;
extern npc_state_t npc_state;

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

#endif
