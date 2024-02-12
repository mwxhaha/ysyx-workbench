#ifdef alu
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <util/log.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>
#include <verilated.h>
#include <Vtop.h>

#define ALU_TEST_LOOP(X)                        \
    do                                          \
    {                                           \
        uint32_t i = 0;                         \
        bool i_last_flag = true;                \
        while (i_last_flag)                     \
        {                                       \
            uint32_t j = 0;                     \
            bool j_last_flag = true;            \
            while (j_last_flag)                 \
            {                                   \
                top->alu_a = i;                 \
                top->alu_b = j;                 \
                update();                       \
                assert(top->alu_result == (X)); \
                if (j == 0xffffffff)            \
                    j_last_flag = false;        \
                j += 0x11111111;                \
            }                                   \
            if (i == 0xffffffff)                \
                i_last_flag = false;            \
            i += 0x11111111;                    \
        }                                       \
    } while (0)

void sim()
{
#ifdef NV_SIM
    while (!contextp->gotFinish())
    {
        update();
    }
#else
    int sim_time = 10000;
    while (contextp->time() < sim_time && !contextp->gotFinish())
    {
        top->alu_funct = 0;
        ALU_TEST_LOOP(i + j);
        top->alu_funct = 8;
        ALU_TEST_LOOP(i - j);
        top->alu_funct = 10;
        ALU_TEST_LOOP((int32_t)i < (int32_t)j);
        top->alu_funct = 11;
        ALU_TEST_LOOP(i < j);
        top->alu_funct = 1;
        ALU_TEST_LOOP(i << (j & 0x1f));
        top->alu_funct = 5;
        ALU_TEST_LOOP(i >> (j & 0x1f));
        top->alu_funct = 13;
        ALU_TEST_LOOP((int32_t)i >> (j & 0x1f));
        top->alu_funct = 4;
        ALU_TEST_LOOP(i ^ j);
        top->alu_funct = 6;
        ALU_TEST_LOOP(i | j);
        top->alu_funct = 7;
        ALU_TEST_LOOP(i & j);
        top->alu_funct = 9;
        ALU_TEST_LOOP(i == j);
        top->alu_funct = 12;
        ALU_TEST_LOOP(i != j);
        top->alu_funct = 14;
        ALU_TEST_LOOP((int32_t)i >= (int32_t)j);
        top->alu_funct = 15;
        ALU_TEST_LOOP(i >= j);
    }
#endif
}
#endif
