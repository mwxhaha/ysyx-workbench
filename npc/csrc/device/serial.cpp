#include <device/serial.hpp>

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
#include <device/map.hpp>
#include <device/mmio.hpp>

#define CH_OFFSET 0
#define CONFIG_SERIAL_MMIO 0xa00003f8

static uint8_t *serial_base = NULL;

static void serial_putc(char ch)
{
    putc(ch, stderr);
}

static void serial_io_handler(uint32_t offset, int len, bool is_write)
{
    assert(len == 1);
    switch (offset)
    {
    /* We bind the serial port with the host stderr in NEMU. */
    case CH_OFFSET:
        if (is_write)
            serial_putc(serial_base[0]);
        else
            panic("do not support read");
        break;
    default:
        panic("do not support offset = %d", offset);
    }
}

void init_serial()
{
    serial_base = new_space(8);
    add_mmio_map("serial", CONFIG_SERIAL_MMIO, serial_base, 8, serial_io_handler);
}
