#include <mem/vaddr.hpp>

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
#include <mem/paddr.hpp>
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

word_t addr_montior_read(vaddr_t addr, int len)
{
    enable_mem_align_check = false;
    mtrace_enable = false;
    dtrace_enable = false;
    enable_device_fresh = false;
    enable_device_skip_diff = false;
    word_t ret = vaddr_read(addr, len);
    enable_mem_align_check = true;
    mtrace_enable = true;
    dtrace_enable = true;
    enable_device_fresh = true;
    enable_device_skip_diff = true;
    return ret;
}

word_t addr_ifetch(vaddr_t addr, int len)
{
    mtrace_enable = false;
    dtrace_enable = false;
    enable_device_fresh = false;
    enable_device_skip_diff = false;
    word_t ret = vaddr_ifetch(addr, len);
    mtrace_enable = true;
    dtrace_enable = true;
    enable_device_fresh = true;
    enable_device_skip_diff = true;
    return ret;
}

word_t addr_read(vaddr_t addr, int len)
{
    return vaddr_read(addr, len);
}

void addr_write(vaddr_t addr, int len, word_t data)
{
    vaddr_write(addr, len, data);
}
