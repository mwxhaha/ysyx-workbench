#include <cpu_cpu_exec.hpp>
#include <sim_tool.hpp>
#include <cstdio>
#include <cstdbool>
#include <cstdint>
#include <cstring>
#include <Vtop___024root.h>
#include <cpu_disasm.hpp>

#define MAX_INST_TO_PRINT 10
static uint64_t g_nr_guest_inst = 0;
static bool g_print_step = false;

static void trace_and_difftest(Decode *_this, vaddr_t dnpc)
{
    if (g_print_step)
    {
        puts(_this->logbuf);
    }
    //   add_iringbuf(_this->logbuf);
    //   IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));
    // #ifdef CONFIG_WATCHPOINT
    //   if (check_watchpoint()) {
    //     printf("watchpoint trigger at:");
    //     puts(_this->logbuf);
    //     if (nemu_state.state == NEMU_RUNNING) nemu_state.state = NEMU_STOP;
    //   }
    // #endif
}

static void exec_once(Decode *s, vaddr_t pc)
{
    s->pc = pc;
    s->isa.inst.val = top->rootp->cpu__DOT__mem_r_1;
    cycle();
    s->snpc = pc+4;
    s->dnpc = top->rootp->cpu__DOT__pc_out;
    char *p = s->logbuf;
    p += snprintf(p, sizeof(s->logbuf), FMT_WORD ":", s->pc);
    int ilen = s->snpc - s->pc;
    int i;
    uint8_t *inst = (uint8_t *)&s->isa.inst.val;
    for (i = ilen - 1; i >= 0; i--)
    {
        p += snprintf(p, 4, " %02x", inst[i]);
    }
    int ilen_max = 4;
    int space_len = ilen_max - ilen;
    if (space_len < 0)
        space_len = 0;
    space_len = space_len * 3 + 1;
    memset(p, ' ', space_len);
    p += space_len;
    disassemble(p, s->logbuf + sizeof(s->logbuf) - p, s->pc, (uint8_t *)&s->isa.inst.val, ilen);
}

static void execute(uint64_t n)
{
    Decode s;
    for (; n > 0; n--)
    {
        exec_once(&s,top->rootp->cpu__DOT__pc_out);
        g_nr_guest_inst++;
        trace_and_difftest(&s,top->rootp->cpu__DOT__pc_out);
        if (npc_state.state != npc_running)
            break;
    }
}

void cpu_exec(uint64_t n)
{
    g_print_step = (n < MAX_INST_TO_PRINT);
    switch (npc_state.state)
    {
    case npc_end:
    case npc_absort:
        printf(
            "Program execution has ended. To restart the program, exit NEMU and "
            "run again.\n");
        return;
    default:
        npc_state.state = npc_running;
    }

    execute(n);

    switch (npc_state.state)
    {
    case npc_running:
        npc_state.state = npc_stop;
        break;
    case npc_end:
    case npc_absort:
        printf("npc: %s at pc = " FMT_WORD "\n",
               npc_state.state == npc_absort ? "ABORT" : (npc_state.ret == 0 ? "HIT GOOD TRAP" : "HIT BAD TRAP"),
               npc_state.pc);
        if (npc_state.state != npc_end || npc_state.ret != 0)
        {
            // isa_reg_display();
            // print_iringbuf();
            // print_ftrace();
        }
    }
}
