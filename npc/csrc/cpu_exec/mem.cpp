#include <cpu_exec/mem.hpp>

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
#include <util/debug.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>

uint8_t pmem[MEM_MAX] = {0x97, 0x14, 0x00, 0x00,  // auipc 9 4096
                         0xb3, 0x86, 0xb4, 0x00,  // add 13 9 11
                         0xa3, 0xaf, 0x96, 0xfe,  // sw 9 -1(13)
                         0x83, 0xa5, 0xf4, 0xff,  // lw 11 -1(9)
                         0x63, 0x84, 0xb6, 0x00,  // beq 11 13 4
                         0xef, 0x04, 0x40, 0x00,  // jal 9 4
                         0x73, 0x00, 0x10, 0x00}; // ebreak

static bool enable_mtrace = true;
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

static void in_pmem(paddr_t addr)
{
    Assert(addr >= MEM_BASE_ADDR, "address = " FMT_PADDR " is out of bound of pmem at pc = " FMT_WORD, addr, TOP_PC);
    Assert(addr <= MEM_BASE_ADDR + MEM_MAX - 1, "address = " FMT_PADDR "is out of bound of pmem at pc = " FMT_WORD, addr, TOP_PC);
}

static uint8_t *guest_to_host(paddr_t paddr)
{
    return pmem + paddr - MEM_BASE_ADDR;
}

static word_t host_read(void *addr, int len)
{
    switch (len)
    {
    case 1:
        return *(uint8_t *)addr;
    case 2:
        return *(uint16_t *)addr;
    case 4:
        return *(uint32_t *)addr;
#if ISA_WIDTH == 64
    case 8:
        return *(uint64_t *)addr;
#endif
    default:
        panic("memory read len error");
    }
}

static void host_write(void *addr, int len, word_t data)
{
    switch (len)
    {
    case 1:
        *(uint8_t *)addr = data;
        return;
    case 2:
        *(uint16_t *)addr = data;
        return;
    case 4:
        *(uint32_t *)addr = data;
        return;
#if ISA_WIDTH == 64
    case 8:
        *(uint64_t *)addr = data;
        return;
#endif
        panic("memory write len error");
    }
}

#ifdef CONFIG_MTRACE
static void mtrace_record(bool is_read, paddr_t addr, int len, word_t read_data, word_t write_data)
{
    if (enable_mtrace && addr >= 0x80000000 && addr <= 0x8fffffff)
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
    enable_mtrace = true;
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

void disable_mtrace_once()
{
    enable_mtrace = false;
}

word_t pmem_read(paddr_t addr, int len)
{
    in_pmem(addr);
    word_t ret = host_read(guest_to_host(addr), len);
#ifdef CONFIG_MTRACE
    mtrace_record(true, addr, len, ret, 0);
#endif
    return ret;
}

void pmem_write(paddr_t addr, int len, word_t data)
{
    in_pmem(addr);
#ifdef CONFIG_MTRACE
    mtrace_record(false, addr, len, host_read(guest_to_host(addr), len), data);
#endif
    host_write(guest_to_host(addr), len, data);
}
