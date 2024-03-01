#include <cpu_exec/reg.hpp>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <sim/cpu.hpp>
#include <util/log.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>

static const char *regs[] = {"$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
                             "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
                             "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
                             "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

static int check_reg_idx(int idx)
{
    assert(idx >= 0 && idx < GPR_NUM);
    return idx;
}

#define gpr(idx) (TOP_GPR[check_reg_idx(idx)])

static const char *reg_name(int idx)
{
    return regs[check_reg_idx(idx)];
}

#define COLUMN 8

void isa_reg_display()
{
    for (int i = 0; i < GPR_NUM; i++)
    {
        printf("%s:" FMT_WORD " ", reg_name(i), gpr(i));
        if (i % COLUMN == COLUMN - 1)
            printf("\n");
    }
    if (GPR_NUM % COLUMN != 0)
        printf("\n");
    printf("%s:" FMT_WORD "\n", "pc", TOP_PC);
}

word_t isa_reg_str2val(const char *s, bool *success)
{
    if (strcmp(s, "pc") == 0)
        return TOP_PC;
    for (int i = 0; i < GPR_NUM; i++)
    {
        if (strcmp(s, reg_name(i)) == 0)
            return gpr(i);
    }
    *success = false;
    return 0;
}

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc)
{
    bool ret = true;
    if (TOP_PC != ref_r->pc)
    {
        printf("pc: " FMT_WORD " is different, it should be " FMT_WORD "\n", TOP_PC, ref_r->pc);
        ret = false;
    }
    for (int i = 0; i < GPR_NUM; i++)
        if (gpr(i) != ref_r->gpr[check_reg_idx(i)])
        {
            printf("reg %s: " FMT_WORD " is different, it should be " FMT_WORD "\n", reg_name(i), gpr(i), ref_r->gpr[i]);
            ret = false;
        }
    if (!ret)
        printf("In pc: " FMT_WORD "\n", pc);
    return ret;
}
