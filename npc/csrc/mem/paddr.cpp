#include <mem/paddr.hpp>

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
#include <mem/host.hpp>
#include <device/mmio.hpp>

static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};

bool in_pmem(paddr_t addr)
{
    return addr - CONFIG_MBASE < CONFIG_MSIZE;
}

uint8_t *guest_to_host(paddr_t paddr)
{
    return pmem + paddr - CONFIG_MBASE;
}

typedef struct
{
    vaddr_t pc;
    bool is_read;
    paddr_t addr;
    int len;
    word_t read_data;
    word_t write_data;
} mtrace_t;
#define MTRACE_ARRAY_MAX 20
static mtrace_t mtrace_array[MTRACE_ARRAY_MAX];
static int mtrace_array_end = 0;
static bool mtrace_array_is_full = false;
bool mtrace_enable = true;

#ifdef CONFIG_MTRACE
static void mtrace_record(bool is_read, paddr_t addr, int len, word_t read_data, word_t write_data)
{
    if (addr >= 0x80000000 && addr <= 0x8fffffff)
    {
        mtrace_array[mtrace_array_end].pc = TOP_PC;
        mtrace_array[mtrace_array_end].is_read = is_read;
        mtrace_array[mtrace_array_end].addr = addr;
        mtrace_array[mtrace_array_end].len = len;
        mtrace_array[mtrace_array_end].read_data = read_data;
        mtrace_array[mtrace_array_end].write_data = write_data;
        mtrace_array_end++;
        if (mtrace_array_end >= MTRACE_ARRAY_MAX)
        {
            mtrace_array_end = 0;
            mtrace_array_is_full = true;
        }
    }
}
#endif

static void printf_mtrace_once(int i)
{
    if (mtrace_array[i].is_read)
    {
        printf(FMT_WORD ": memory read  addr = " FMT_WORD " len = %d: " FMT_WORD "\n", mtrace_array[i].pc, mtrace_array[i].addr, mtrace_array[i].len, mtrace_array[i].read_data);
    }
    else
    {
        printf(FMT_WORD ": memory write addr = " FMT_WORD " len = %d: " FMT_WORD " -> " FMT_WORD "\n", mtrace_array[i].pc, mtrace_array[i].addr, mtrace_array[i].len, mtrace_array[i].read_data, mtrace_array[i].write_data);
    }
}

void print_mtrace()
{
    if (!mtrace_array_is_full && mtrace_array_end == 0)
    {
        printf("mtrace is empty now\n");
        return;
    }
    if (mtrace_array_is_full)
    {
        int i = mtrace_array_end;
        printf_mtrace_once(i);
        i++;
        if (i == MTRACE_ARRAY_MAX)
            i = 0;
        while (i != mtrace_array_end)
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
        while (i != mtrace_array_end)
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
    if (mtrace_enable)
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
    panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD, addr, PMEM_LEFT, PMEM_RIGHT, TOP_PC);
}

void init_mem()
{
#ifdef CONFIG_MEM_RANDOM
    uint32_t *p = (uint32_t *)pmem;
    int i;
    for (i = 0; i < (int)(CONFIG_MSIZE / sizeof(p[0])); i++)
    {
        p[i] = rand();
    }
#endif
    Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
}

void mem_quit()
{
}

#if (CONFIG_ISA == CONFIG_RV32I || CONFIG_ISA == CONFIG_RV32E || CONFIG_ISA == CONFIG_RV64I)
static const uint32_t img[] = {0x00001497,  // auipc 9 4096
                               0x00b486b3,  // add 13 9 11
                               0xfe96ae23,  // sw 9 -4(13)
                               0xffc4a583,  // lw 11 -4(9)
                               0x00b68463,  // beq 11 13 4
                               0x004004ef,  // jal 9 4
                               0x00100073}; // ebreak
#else
#error("use other init default img");
#endif

void init_isa()
{
    memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));
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
#if ISA_WIDTH == 64
    case 8:
        if ((addr & 0x7) != 0)
            is_mem_align = false;
        break;
#endif
    }
    if (!is_mem_align)
        panic("address = " FMT_PADDR " len = %d is unalign at pc = " FMT_WORD, addr, len, TOP_PC);
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
