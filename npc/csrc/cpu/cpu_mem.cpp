#include <cpu/cpu_mem.hpp>

#include <cstdio>
#include <cstdint>
#include <cstdbool>

#include <sim/cpu_sim.hpp>
#include <util/debug.hpp>

uint8_t mem[MEM_MAX] = {0x97, 0x14, 0x00, 0x00,  // auipc 9 4096
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
        printf("memory read in addr " FMT_WORD " : " FMT_WORD "\n", mtrace_array[i].addr, mtrace_array[i].read_data);
    }
    else
    {
        printf("memory write in addr " FMT_WORD " with mask 0x%04x: " FMT_WORD "->" FMT_WORD "\n", mtrace_array[i].addr, mtrace_array[i].len, mtrace_array[i].read_data, mtrace_array[i].write_data);
    }
}

void print_mtrace()
{
    if (!mtrace_array_is_full && mtrace_array_tail==0)
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

void pmem_read(paddr_t raddr, word_t *rdata)
{
#if (ISA_WIDTH == 64)
    panic("do not isa64");
#else
    Assert(raddr >= MEM_BASE_ADDR, "memory out of bound");
    Assert(raddr <= MEM_BASE_ADDR + MEM_MAX - 1, "memory out of bound");
    word_t *addr_real = (word_t *)(intptr_t)raddr - (word_t *)MEM_BASE_ADDR + (word_t *)mem;
    *rdata = *addr_real;
#ifdef CONFIG_MTRACE
    mtrace_record(true, raddr, 4, *rdata, 0);
#endif
#endif
}

void pmem_write(paddr_t waddr, word_t wdata, uint8_t wmask)
{
#if (ISA_WIDTH == 64)
    panic("do not isa64");
#else
    Assert(waddr >= MEM_BASE_ADDR, "memory out of bound");
    Assert(waddr <= MEM_BASE_ADDR + MEM_MAX - 1, "memory out of bound");
    word_t *addr_real = (word_t *)(intptr_t)waddr - (word_t *)MEM_BASE_ADDR + (word_t *)mem;
#ifdef CONFIG_MTRACE
    mtrace_record(false, waddr, wmask, *addr_real, wdata);
#endif
    switch (wmask)
    {
    case 1:
        *(uint8_t *)addr_real = wdata;
        break;
    case 3:
        *(uint16_t *)addr_real = wdata;
        break;
    case 15:
        *(uint32_t *)addr_real = wdata;
        break;
    default:
        panic("memory write mask error");
        break;
    }
#endif
}
