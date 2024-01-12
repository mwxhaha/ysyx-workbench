#include <cpu/cpu_log.hpp>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <sim/cpu_sim.hpp>
#include <util/debug.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>

extern uint64_t g_nr_guest_inst;
FILE *log_fp = NULL;
static bool log_close = false;

void init_log(const char *log_file)
{
    if (!log_file)
    {
        printf(ANSI_FMT("No log file is given. Log will not work\n", ANSI_FG_BLUE));
        log_close = true;
        return;
    }
        FILE *fp = fopen(log_file, "w");
        Assert(fp, "Can not open '%s'", log_file);
        log_fp = fp;
    Log("Log is written to %s", log_file ? log_file : "stdout");
}

bool log_enable()
{
    return MUXDEF(CONFIG_TRACE, (g_nr_guest_inst >= 0) && (g_nr_guest_inst <= 10000), false);
}
