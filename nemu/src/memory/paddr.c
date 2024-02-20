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

#include <memory/host.h>
#include <common.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>
#include <stdbool.h>

#if defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
#endif

uint8_t *guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

bool enable_mtrace = true;
typedef struct
{
    bool is_read;
    paddr_t addr;
    int len;
    word_t read_data;
    word_t write_data;
} mtrace_t;
#define MTRACE_ARRAY_MAX 20
static mtrace_t mtrace_array[MTRACE_ARRAY_MAX];
static int mtrace_array_tail = 0;
static bool mtrace_array_is_full = false;

#ifdef CONFIG_MTRACE
static void mtrace_record(bool is_read, paddr_t addr, int len, word_t read_data, word_t write_data)
{
    if (addr >= 0x80000000 && addr <= 0x8fffffff)
    {
        mtrace_array[mtrace_array_tail].is_read = is_read;
        mtrace_array[mtrace_array_tail].addr = addr;
        mtrace_array[mtrace_array_tail].len = len;
        mtrace_array[mtrace_array_tail].read_data = read_data;
        mtrace_array[mtrace_array_tail].write_data = write_data;
        mtrace_array_tail++;
        if (mtrace_array_tail >= MTRACE_ARRAY_MAX)
        {
            mtrace_array_tail = 0;
            mtrace_array_is_full = true;
        }
    }
}
#endif

static void printf_mtrace_once(int i)
{
    if (mtrace_array[i].is_read)
    {
        printf("memory read in addr " FMT_WORD " with len %d: " FMT_WORD "\n", mtrace_array[i].addr, mtrace_array[i].len, mtrace_array[i].read_data);
    }
    else
    {
        printf("memory write in addr " FMT_WORD " with len %d: " FMT_WORD "->" FMT_WORD "\n", mtrace_array[i].addr, mtrace_array[i].len, mtrace_array[i].read_data, mtrace_array[i].write_data);
    }
}

void print_mtrace()
{
    if (!mtrace_array_is_full && mtrace_array_tail == 0)
    {
        printf("mtrace is empty now\n");
        return;
    }
    if (mtrace_array_is_full)
    {
        int i = mtrace_array_tail;
        printf_mtrace_once(i);
        i++;
        if (i == MTRACE_ARRAY_MAX)
            i = 0;
        while (i != mtrace_array_tail)
        {
            printf_mtrace_once(i);
            i++;
            if (i == MTRACE_ARRAY_MAX)
                i = 0;
        }
    }
    else
    {
        int i = 0;
        while (i != mtrace_array_tail)
        {
            printf_mtrace_once(i);
            i++;
        }
    }
}

static word_t pmem_read(paddr_t addr, int len)
{
    word_t ret = host_read(guest_to_host(addr), len);
#ifdef CONFIG_MTRACE
    if (enable_mtrace)
        mtrace_record(true, addr, len, ret, 0);
#endif
    return ret;
}

static void pmem_write(paddr_t addr, int len, word_t data)
{
#ifdef CONFIG_MTRACE
    mtrace_record(false, addr, len, host_read(guest_to_host(addr), len), data);
#endif
    host_write(guest_to_host(addr), len, data);
}

static void out_of_bound(paddr_t addr)
{
    panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
          addr, PMEM_LEFT, PMEM_RIGHT, cpu.pc);
}

void init_mem()
{
#if defined(CONFIG_PMEM_MALLOC)
    pmem = malloc(CONFIG_MSIZE);
    assert(pmem);
#endif
#ifdef CONFIG_MEM_RANDOM
    uint32_t *p = (uint32_t *)pmem;
    int i;
    for (i = 0; i < (int)(CONFIG_MSIZE / sizeof(p[0])); i++)
    {
        p[i] = rand();
    }
#endif
#ifdef CONFIG_TARGET_NATIVE_ELF
    Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
#endif
}

void mem_quit()
{
#if defined(CONFIG_PMEM_MALLOC)
    free(pmem);
#endif
}

bool enable_mem_align_check = true;

static void mem_align_check(paddr_t addr, int len)
{
    bool is_mem_align = true;
    switch (len)
    {
    case 2:
        if ((addr & 0x1) != 0)
            is_mem_align = false;
        break;
    case 4:
        if ((addr & 0x3) != 0)
            is_mem_align = false;
        break;
#ifdef CONFIG_ISA64
    case 8:
        if ((addr & 0x7) != 0)
            is_mem_align = false;
        break;
#endif
    }
    if (!is_mem_align)
        panic("address = " FMT_PADDR " len = %d is unalign at pc = " FMT_WORD, addr, len, cpu.pc);
}

word_t paddr_read(paddr_t addr, int len)
{
    if (enable_mem_align_check)
        mem_align_check(addr, len);
    if (likely(in_pmem(addr)))
        return pmem_read(addr, len);
    IFDEF(CONFIG_DEVICE, return mmio_read(addr, len));
    out_of_bound(addr);
    return 0;
}

void paddr_write(paddr_t addr, int len, word_t data)
{
    if (enable_mem_align_check)
        mem_align_check(addr, len);
    if (likely(in_pmem(addr)))
    {
        pmem_write(addr, len, data);
        return;
    }
    IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
    out_of_bound(addr);
}
