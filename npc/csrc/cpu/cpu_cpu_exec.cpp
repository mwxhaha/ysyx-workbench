#include <cpu/cpu_cpu_exec.hpp>

#include <cstdio>
#include <cstdbool>
#include <cstdint>
#include <cstring>
#include <locale.h>

#include <util/sim_tool.hpp>
#include <util/timer.hpp>
#include <util/disasm.hpp>
#include <util/macro.hpp>
#include <sim/cpu_sim.hpp>
#include <Vtop___024root.h>
#include <cpu/cpu_reg.hpp>
#include <cpu/cpu_mem.hpp>
#include <monitor/cpu_watchpoint.hpp>
#include <cpu/cpu_iringbuf.hpp>
#include <cpu/cpu_ftrace.hpp>
#include <cpu/cpu_dut.hpp>

#define MAX_INST_TO_PRINT 20
uint64_t g_nr_guest_inst = 0;
static uint64_t g_timer = 0; // unit: us
static bool g_print_step = false;

static void trace_and_difftest(Decode *_this, vaddr_t dnpc)
{
#ifdef CONFIG_ITRACE
    if (g_print_step)
        puts(_this->logbuf);
    add_iringbuf(_this->logbuf);
#endif
    IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));
#ifdef CONFIG_WATCHPOINT
    if (check_watchpoint())
    {
        printf("watchpoint trigger at: ");
        MUXDEF(CONFIG_ITRACE, puts(_this->logbuf), printf(FMT_WORD ", inst: " FMT_INST "\n", _this->pc, _this->isa.inst.val));
        if (npc_state.state == npc_running)
            npc_state.state = npc_stop;
    }
#endif
}

static void exec_once(Decode *s, vaddr_t pc)
{
    s->pc = pc;
    s->isa.inst.val = top->rootp->cpu__DOT__mem_r_1;
    cycle(1, CYCLE);
    s->snpc = pc + INST_LEN / 8;
    s->dnpc = top->rootp->cpu__DOT__pc_out;
    if ((s->isa.inst.val & 0x7f) == 0x6f || (s->isa.inst.val & 0x707f) == 0x67)
        ftrace_record(s);
#ifdef CONFIG_ITRACE
    char *p = s->logbuf;
    p += snprintf(p, sizeof(s->logbuf), FMT_WORD ":", s->pc);
    int ilen = s->snpc - s->pc;
    int i;
    uint8_t *inst = (uint8_t *)&s->isa.inst.val;
    for (i = ilen - 1; i >= 0; i--)
    {
        p += snprintf(p, 4, " %02x", inst[i]);
    }
    int ilen_max = INST_LEN / 8;
    int space_len = ilen_max - ilen;
    if (space_len < 0)
        space_len = 0;
    space_len = space_len * 3 + 1;
    memset(p, ' ', space_len);
    p += space_len;
    disassemble(p, s->logbuf + sizeof(s->logbuf) - p, s->pc, (uint8_t *)&s->isa.inst.val, ilen);
#endif
}

static void execute(uint64_t n)
{
    Decode s;
    for (; n > 0; n--)
    {
        exec_once(&s, top->rootp->cpu__DOT__pc_out);
        g_nr_guest_inst++;
        trace_and_difftest(&s, top->rootp->cpu__DOT__pc_out);
        if (npc_state.state != npc_running)
            break;
    }
}

static void statistic()
{
    setlocale(LC_NUMERIC, "");
#define NUMBERIC_FMT "%'lu"
    printf("host time spent = " NUMBERIC_FMT " us\n", g_timer);
    printf("total guest instructions = " NUMBERIC_FMT "\n", g_nr_guest_inst);
    if (g_timer > 0)
        printf("simulation frequency = " NUMBERIC_FMT " inst/s\n",
               g_nr_guest_inst * 1000000 / g_timer);
    else
        printf("Finish running in less than 1 us and can not calculate the simulation "
               "frequency\n");
}

void assert_fail_msg()
{
    isa_reg_display();
    IFDEF(CONFIG_ITRACE, print_iringbuf());
    IFDEF(CONFIG_MTRACE, print_mtrace());
    IFDEF(CONFIG_FTRACE, print_ftrace());
    statistic();
}

void cpu_exec(uint64_t n)
{
    g_print_step = (n <= MAX_INST_TO_PRINT);
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

    uint64_t timer_start = get_time();

    execute(n);

    uint64_t timer_end = get_time();
    g_timer += timer_end - timer_start;

    switch (npc_state.state)
    {
    case npc_running:
    case npc_stop:
        npc_state.state = npc_stop;
        break;
    case npc_end:
    case npc_absort:
        printf("npc: %s at pc = " FMT_WORD "\n",
               npc_state.state == npc_absort ? "ABORT" : (npc_state.ret == 0 ? "HIT GOOD TRAP" : "HIT BAD TRAP"),
               npc_state.pc);
        if (npc_state.state != npc_end || npc_state.ret != 0)
        {
            isa_reg_display();
            IFDEF(CONFIG_ITRACE, print_iringbuf());
            IFDEF(CONFIG_MTRACE, print_mtrace());
            IFDEF(CONFIG_FTRACE, print_ftrace());
        }
    case npc_quit:
        statistic();
    }
}
