#include <cpu_exec/vaddr.hpp>

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
#include <cpu_exec/mem.hpp>
#include <device/map.hpp>

static word_t vaddr_ifetch(vaddr_t addr, int len)
{
    return paddr_read(addr, len);
}

static word_t vaddr_read(vaddr_t addr, int len)
{
    return paddr_read(addr, len);
}

static void vaddr_write(vaddr_t addr, int len, word_t data)
{
    paddr_write(addr, len, data);
}

static void addr_ifetch_bus_align(vaddr_t addr, word_t *data)
{
    *data = vaddr_ifetch(addr, 4);
}

static void addr_read_bus_align(vaddr_t addr, word_t *data)
{
    *data = vaddr_read(addr, 4);
}

static void addr_write_bus_align(vaddr_t addr, uint8_t mask, word_t data)
{
    vaddr_t addr_shift;
    char mask_shift;
    word_t data_shift;
    if (((mask & 0x1) == 1))
    {
        addr_shift = addr;
        mask_shift = mask;
        data_shift = data;
    }
    else if ((mask & 0x2) == 2)
    {
        addr_shift = addr + 1;
        mask_shift = mask >> 1;
        data_shift = data >> 8;
    }
    else if ((mask & 0x4) == 4)
    {
        addr_shift = addr + 2;
        mask_shift = mask >> 2;
        data_shift = data >> 16;
    }
    else if ((mask & 0x8) == 8)
    {
        addr_shift = addr + 3;
        mask_shift = mask >> 3;
        data_shift = data >> 24;
    }
    else
    {
        panic("memory write mask 0x%x bus align error at addr = " FMT_PADDR " at pc = " FMT_WORD, mask, addr, TOP_PC);
    }

    switch (mask_shift)
    {
    case 0x1:
        vaddr_write(addr_shift, 1, data_shift);
        break;
    case 0x3:
        vaddr_write(addr_shift, 2, data_shift);
        break;
    case 0xf:
        vaddr_write(addr_shift, 4, data_shift);
        break;
    default:
        panic("memory write mask 0x%x change to len error at addr = " FMT_PADDR " at pc = " FMT_WORD, mask, addr, TOP_PC);
    }
}

word_t addr_montior_read(vaddr_t addr, int len)
{
    enable_mem_align_check = false;
    enable_mtrace = false;
    enable_dtrace = false;
    enable_device_fresh = false;
    enable_device_skip_diff = false;
    word_t ret = vaddr_read(addr, len);
    enable_mem_align_check = true;
    enable_mtrace = true;
    enable_dtrace = true;
    enable_device_fresh = true;
    enable_device_skip_diff = true;
    return ret;
}

void addr_ifetch(vaddr_t addr, word_t *data)
{
    enable_mtrace = false;
    enable_dtrace = false;
    enable_device_fresh = false;
    enable_device_skip_diff = false;
    addr_ifetch_bus_align(addr, data);
    enable_mtrace = true;
    enable_dtrace = true;
    enable_device_fresh = true;
    enable_device_skip_diff = true;
}

void addr_read(vaddr_t addr, word_t *data)
{
    addr_read_bus_align(addr, data);
}

void addr_write(vaddr_t addr, uint8_t mask, word_t data)
{
    addr_write_bus_align(addr, mask, data);
}
