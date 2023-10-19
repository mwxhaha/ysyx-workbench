#include <sdb/cpu_reg.hpp>
#include <util/sim_tool.hpp>
#include <Vtop.h>
#include <Vtop___024root.h>
#include <sim/cpu_sim.hpp>

const char *regs[] = {"$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
                      "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
                      "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
                      "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

void isa_reg_display()
{
    int column_cnt_display = 8;
    for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++)
    {
        printf("%s:" FMT_WORD " ", regs[i], top->rootp->cpu__DOT__gpr_1__DOT__registerfile_gpr__DOT__rf[i]);
        if (i % column_cnt_display == column_cnt_display - 1)
            printf("\n");
    }
    if (sizeof(regs) / sizeof(regs[0]) % column_cnt_display != 0)
        printf("\n");
    printf("%s:" FMT_WORD "\n", "pc", top->rootp->cpu__DOT__pc_out);
}

word_t isa_reg_str2val(const char *s, bool *success)
{
    if (strcmp(s, "pc") == 0)
        return top->rootp->cpu__DOT__pc_out;
    for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++)
    {
        if (strcmp(s, regs[i]) == 0)
            return top->rootp->cpu__DOT__gpr_1__DOT__registerfile_gpr__DOT__rf[i];
    }
    *success = false;
    return 0;
}