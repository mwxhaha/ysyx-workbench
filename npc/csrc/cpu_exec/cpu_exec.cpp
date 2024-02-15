#include <cpu_exec/cpu_exec.hpp>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>

#include <sim/cpu.hpp>
#include <util/log.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>
#include <util/timer.hpp>
#include <util/disasm.hpp>
#include <cpu_exec/dut.hpp>
#include <cpu_exec/ftrace.hpp>
#include <cpu_exec/iringbuf.hpp>
#include <cpu_exec/mem.hpp>
#include <cpu_exec/reg.hpp>
#include <monitor/watchpoint.hpp>
#include <device/device.hpp>

#define MAX_INST_TO_PRINT 20
uint64_t g_nr_guest_inst = 0;
static uint64_t g_timer = 0; // unit: us
static bool g_print_step = false;

static void trace_and_difftest(Decode *_this, vaddr_t dnpc)
{
#ifdef CONFIG_ITRACE
    log_write("%s\n", _this->logbuf);
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
        if (npc_state.state == NPC_RUNNING)
            npc_state.state = NPC_STOP;
    }
#endif
}

static void exec_once(Decode *s, vaddr_t pc)
{
    s->pc = pc;
    s->snpc = pc + INST_LEN / 8;
    s->isa.inst.val = TOP_INST;
    cycle(1, CYCLE);
    s->dnpc = TOP_PC;
#ifdef CONFIG_FTRACE
    if ((s->isa.inst.val & 0x7f) == 0x6f || (s->isa.inst.val & 0x707f) == 0x67)
        ftrace_record(s);
#endif
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
#if (CONFIG_ISA == CONFIG_RV32I || CONFIG_ISA == CONFIG_RV32E || CONFIG_ISA == CONFIG_RV64I)
    disassemble(p, s->logbuf + sizeof(s->logbuf) - p, s->pc, (uint8_t *)&s->isa.inst.val, ilen);
#else
    panic("disassemble do not support other isa");
#endif
#endif
}

static void execute(uint64_t n)
{
    Decode s;
    for (; n > 0; n--)
    {
        exec_once(&s, TOP_PC);
        g_nr_guest_inst++;
        trace_and_difftest(&s, s.dnpc);
        IFDEF(CONFIG_DEVICE, device_update());
        if (npc_state.state != NPC_RUNNING)
            break;
    }
}

static void statistic()
{
    setlocale(LC_NUMERIC, "");
#define NUMBERIC_FMT "%'" PRIu64
    Log("host time spent = " NUMBERIC_FMT " us", g_timer);
    Log("total guest instructions = " NUMBERIC_FMT, g_nr_guest_inst);
    if (g_timer > 0)
        Log("simulation frequency = " NUMBERIC_FMT " inst/s", g_nr_guest_inst * 1000000 / g_timer);
    else
        Log("Finish running in less than 1 us and can not calculate the simulation frequency");
}

void assert_fail_msg()
{
    isa_reg_display();
    IFDEF(CONFIG_ITRACE, print_iringbuf());
    IFDEF(CONFIG_MTRACE, print_mtrace());
    IFDEF(CONFIG_FTRACE, print_ftrace());
    statistic();
    sim_exit();
}

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n)
{
    g_print_step = (n <= MAX_INST_TO_PRINT);
    switch (npc_state.state)
    {
    case NPC_END:
    case NPC_ABORT:
        printf(
            "Program execution has ended. To restart the program, exit NEMU and "
            "run again.\n");
        return;
    default:
        npc_state.state = NPC_RUNNING;
    }

    uint64_t timer_start = get_time();

    execute(n);

    uint64_t timer_end = get_time();
    g_timer += timer_end - timer_start;

    switch (npc_state.state)
    {
    case NPC_RUNNING:
        npc_state.state = NPC_STOP;
        break;
    case NPC_END:
    case NPC_ABORT:
        Log("npc: %s at pc = " FMT_WORD,
            (npc_state.state == NPC_ABORT
                 ? ANSI_FMT("ABORT", ANSI_FG_RED)
                 : (npc_state.halt_ret == 0
                        ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN)
                        : ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
            npc_state.halt_pc);
        if (npc_state.state != NPC_END || npc_state.halt_ret != 0)
        {
            isa_reg_display();
            IFDEF(CONFIG_ITRACE, print_iringbuf());
            IFDEF(CONFIG_MTRACE, print_mtrace());
            IFDEF(CONFIG_FTRACE, print_ftrace());
        }
    case NPC_QUIT:
        statistic();
    }
}
