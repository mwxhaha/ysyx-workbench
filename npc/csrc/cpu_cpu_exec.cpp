#include <cstdio>
#include <cstdbool>
#include <cstdint>
#include <sim_tool.hpp>



static void execute(uint64_t n) {
//   Decode s;
  for (; n > 0; n--) {
      cycle();
      // g_nr_guest_inst++;
      // trace_and_difftest(&s, cpu.pc);
      if (npc_state.state != running)
          break;
      // IFDEF(CONFIG_DEVICE, device_update());
  }
}

void cpu_exec(uint64_t n)
{
    //   g_print_step = (n < MAX_INST_TO_PRINT);
    switch (npc_state.state)
    {
    case end:
    case absort:
        printf(
            "Program execution has ended. To restart the program, exit NEMU and "
            "run again.\n");
        return;
    default:
        npc_state.state = running;
    }

    //   uint64_t timer_start = get_time();

    execute(n);

    //   uint64_t timer_end = get_time();
    //   g_timer += timer_end - timer_start;

    switch (npc_state.state)
    {
    case running:
        npc_state.state = stop;
        break;

    case end:
    case absort:
        // Log("nemu: %s at pc = " FMT_WORD,
        //     (nemu_state.state == NEMU_ABORT
        //          ? ANSI_FMT("ABORT", ANSI_FG_RED)
        //          : (nemu_state.halt_ret == 0
        //                 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN)
        //                 : ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
        //     nemu_state.halt_pc);
        if (npc_state.state != end || npc_state.ret != 0)
        {
            // isa_reg_display();
            // print_iringbuf();
            // print_ftrace();
        }
        // fall through
    case quit:
        // statistic();
        ;
    }
}
