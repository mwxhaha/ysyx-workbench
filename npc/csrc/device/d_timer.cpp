#include <device/d_timer.hpp>

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
#include <util/timer.hpp>
#include <device/map.hpp>
#include <device/mmio.hpp>

#define CONFIG_RTC_MMIO 0xa0000048

static uint32_t *rtc_port_base = NULL;

static void rtc_io_handler(uint32_t offset, int len, bool is_write)
{
    assert(offset == 0 || offset == 4);
    if (!is_write && offset == 4)
    {
        uint64_t us = get_time();
        rtc_port_base[0] = (uint32_t)us;
        rtc_port_base[1] = us >> 32;
    }
}

void init_timer()
{
    rtc_port_base = (uint32_t *)new_space(8);
    add_mmio_map("rtc", CONFIG_RTC_MMIO, rtc_port_base, 8, rtc_io_handler);
}
