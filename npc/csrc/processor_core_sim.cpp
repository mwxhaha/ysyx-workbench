#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#include "sim_tool.hpp"
#include <iostream>
#include <Vtop___024root.h>

int sim()
{
#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        update();
    }
#else
    int sim_time = 1000;
    reset();
    while (contextp->time() < sim_time && !contextp->gotFinish() && ebreak_flag)
    {
        cycle();
    }
    if (!ebreak_flag && top->rootp->processor_core__DOT__gpr_1__DOT__registerfile_gpr__DOT__rf[10] == 0)
    {
        std::cout << "HIT GOOD TRAP" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "HIT BAD TRAP" << std::endl;
        return 1;
    }
#endif
}
