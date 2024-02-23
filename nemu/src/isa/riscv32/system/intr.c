/***************************************************************************************
 * Copyright (c) 2014-2022 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include <isa.h>
#include "../local-include/reg.h"

typedef struct
{
    vaddr_t pc;
    word_t cause;
} etrace_t;
#define ETRACE_ARRAY_MAX 20
static etrace_t etrace_array[ETRACE_ARRAY_MAX];
static int etrace_array_end = 0;
static bool etrace_array_is_full = false;

#ifdef CONFIG_ETRACE
static void etrace_record(vaddr_t pc, word_t cause)
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
#endif

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

word_t isa_raise_intr(word_t NO, vaddr_t epc)
{
    /* TODO: Trigger an interrupt/exception with ``NO''.
     * Then return the address of the interrupt/exception vector.
     */
#ifdef CONFIG_ETRACE
    etrace_record(epc, NO);
#endif
    mepc = epc;
    mcause = NO;
    return mtvec;
}

word_t isa_query_intr()
{
    return INTR_EMPTY;
}
