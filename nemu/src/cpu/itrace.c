#include <cpu/cpu.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ITRACE_ARRAY_MAX 20
static char itrace_array[ITRACE_ARRAY_MAX][128];
static int itrace_array_end = 0;
static bool itrace_array_is_full = false;

void itrace_record(Decode *s)
{
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

#ifndef CONFIG_ISA_loongarch32r
    void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
    disassemble(p, s->logbuf + sizeof(s->logbuf) - p,
                MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc),
                (uint8_t *)&s->isa.inst.val, ilen);
#else
    p[0] = '\0'; // the upstream llvm does not support loongarch32r
#endif

    strcpy(itrace_array[itrace_array_end], s->logbuf);
    itrace_array_end++;
    if (itrace_array_end == ITRACE_ARRAY_MAX)
    {
        itrace_array_end = 0;
        itrace_array_is_full = true;
    }
}

void print_itrace()
{
    if (!itrace_array_is_full && itrace_array_end == 0)
    {
        printf("itrace is empty now\n");
        return;
    }
    if (itrace_array_is_full)
    {
        int i = itrace_array_end;
        puts(itrace_array[i]);
        i++;
        if (i == ITRACE_ARRAY_MAX)
            i = 0;
        while (i != itrace_array_end)
        {
            puts(itrace_array[i]);
            i++;
            if (i == ITRACE_ARRAY_MAX)
                i = 0;
        }
    }
    else
    {
        int i = 0;
        while (i != itrace_array_end)
        {
            puts(itrace_array[i]);
            i++;
        }
    }
}
