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
#include <util/log.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>

uint8_t pmem[CONFIG_MSIZE];

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

static bool in_pmem(paddr_t addr)
{
    // Assert(addr >= CONFIG_MBASE, "address = " FMT_PADDR " is out of bound of pmem at pc = " FMT_WORD, addr, TOP_PC);
    // Assert(addr <= CONFIG_MBASE + CONFIG_MSIZE - 1, "address = " FMT_PADDR "is out of bound of pmem at pc = " FMT_WORD, addr, TOP_PC);
    return addr - CONFIG_MBASE < CONFIG_MSIZE;
}

static uint8_t *guest_to_host(paddr_t paddr)
{
    return pmem + paddr - CONFIG_MBASE;
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

static word_t pmem_read(paddr_t addr, int len)
{
    word_t ret = host_read(guest_to_host(addr), len);
#ifdef CONFIG_MTRACE
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

static const uint32_t img[] = {0x00001497,  // auipc 9 4096
                               0x00b486b3,  // add 13 9 11
                               0xfe96ae23,  // sw 9 -4(13)
                               0xffc4a583,  // lw 11 -4(9)
                               0x00b68463,  // beq 11 13 4
                               0x004004ef,  // jal 9 4
                               0x00100073}; // ebreak
void init_isa()
{
    memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));
}

word_t paddr_read(paddr_t addr, int len)
{
    if (likely(in_pmem(addr)))
        return pmem_read(addr, len);
    // IFDEF(CONFIG_DEVICE, return mmio_read(addr, len));
    out_of_bound(addr);
    return 0;
}

void paddr_write(paddr_t addr, int len, word_t data)
{
    if (likely(in_pmem(addr)))
    {
        pmem_write(addr, len, data);
        return;
    }
    // IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
    out_of_bound(addr);
}
