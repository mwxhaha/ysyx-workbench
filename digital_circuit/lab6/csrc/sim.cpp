#define HEADER_IN(X) <X##_sim.hpp>
#define HEADER(X) HEADER_IN(X)
#define SIM_IN(X) X##_sim()
#define SIM(X) SIM_IN(X)

#include "verilated.h"
#include "Vtop.h"
#include "verilated_vcd_c.h"
#include "sim_tool.hpp"
#include HEADER(TOP_NAME)

VerilatedContext *contextp;
Vtop *top;
VerilatedVcdC *tfp;

int main(int argc, char **argv)
{
    sim_init(argc, argv);

    SIM(TOP_NAME);

    sim_exit();

    return 0;
}


