#include <verilated.h>
#include <Vtop.h>
#include <verilated_vcd_c.h>
#include <sim_tool.hpp>

void sim()
{
#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        update();
    }
#else
    unsigned int sim_time = 4000;
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        for (unsigned int i = 0; i <= 7; ++i)
            for (unsigned int j = 0; j <= 15; ++j)
                for (unsigned int k = 0; k <= 15; ++k)
                {
                    top->a = j;
                    top->b = k;
                    top->func = i;
                    update();
                }
    }
#endif
}
