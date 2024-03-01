#include <cpu_exec/intr.hpp>

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

typedef struct
{
    vaddr_t pc;
    word_t cause;
} etrace_t;
#define ETRACE_ARRAY_MAX 20
static etrace_t etrace_array[ETRACE_ARRAY_MAX];
static int etrace_array_end = 0;
static bool etrace_array_is_full = false;

void etrace_record(vaddr_t pc, word_t cause)
{
    etrace_array[etrace_array_end].pc = pc;
    etrace_array[etrace_array_end].cause = cause;
    etrace_array_end++;
    if (etrace_array_end >= ETRACE_ARRAY_MAX)
    {
        etrace_array_is_full = true;
        etrace_array_end = 0;
    }
}

static void print_etrace_one(int i)
{
    printf(FMT_WORD ": exception cause = " FMT_WORD "\n", etrace_array[i].pc, etrace_array[i].cause);
}

void print_etrace()
{
    if (!etrace_array_is_full && etrace_array_end == 0)
    {
        printf("etrace is empty now\n");
        return;
    }
    if (etrace_array_is_full)
    {
        int i = etrace_array_end;
        print_etrace_one(i);
        i++;
        if (i == ETRACE_ARRAY_MAX)
            i = 0;
        while (i != etrace_array_end)
        {
            print_etrace_one(i);
            i++;
            if (i == ETRACE_ARRAY_MAX)
                i = 0;
        }
    }
    else
    {
        int i = 0;
        while (i != etrace_array_end)
        {
            print_etrace_one(i);
            i++;
        }
    }
}
