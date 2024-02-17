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
#include <memory/host.h>
#include <memory/vaddr.h>
#include <device/map.h>
#include <stdbool.h>

#define IO_SPACE_MAX (2 * 1024 * 1024)

static uint8_t *io_space = NULL;
static uint8_t *p_space = NULL;

uint8_t *new_space(int size)
{
    uint8_t *p = p_space;
    // page aligned;
    size = (size + (PAGE_SIZE - 1)) & ~PAGE_MASK;
    p_space += size;
    assert(p_space - io_space < IO_SPACE_MAX);
    return p;
}

bool enable_device_skip_diff = true;

static void check_bound(IOMap *map, paddr_t addr)
{
    if (map == NULL)
    {
        Assert(map != NULL, "address (" FMT_PADDR ") is out of bound at pc = " FMT_WORD, addr, cpu.pc);
    }
    else
    {
        Assert(addr <= map->high && addr >= map->low,
               "address (" FMT_PADDR ") is out of bound {%s} [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
               addr, map->name, map->low, map->high, cpu.pc);
    }
}

static void invoke_callback(io_callback_t c, paddr_t offset, int len, bool is_write)
{
    if (c != NULL)
    {
        c(offset, len, is_write);
    }
}

void init_map()
{
    io_space = malloc(IO_SPACE_MAX);
    assert(io_space);
    p_space = io_space;
}

void map_quit()
{
    free(io_space);
}

bool enable_dtrace = true;
typedef struct
{
    bool is_read;
    paddr_t addr;
    int len;
    IOMap *map;
    word_t read_data;
    word_t write_data;
} dtrace_t;
#define DTRACE_ARRAY_MAX 20
static dtrace_t dtrace_array[DTRACE_ARRAY_MAX];
static int dtrace_array_tail = 0;
static bool dtrace_array_is_full = false;

#ifdef CONFIG_DTRACE
static void dtrace_record(bool is_read, paddr_t addr, int len, IOMap *map, word_t read_data, word_t write_data)
{
    if (enable_dtrace && addr >= 0x8fffffff && addr <= 0xffffffff)
    {
        dtrace_array[dtrace_array_tail].is_read = is_read;
        dtrace_array[dtrace_array_tail].addr = addr;
        dtrace_array[dtrace_array_tail].len = len;
        dtrace_array[dtrace_array_tail].map = map;
        dtrace_array[dtrace_array_tail].read_data = read_data;
        dtrace_array[dtrace_array_tail].write_data = write_data;
        dtrace_array_tail++;
        if (dtrace_array_tail >= DTRACE_ARRAY_MAX)
        {
            dtrace_array_tail = 0;
            dtrace_array_is_full = true;
        }
    }
}
#endif

static void printf_dtrace_once(int i)
{
    if (dtrace_array[i].is_read)
    {
        printf("device %s read in addr " FMT_WORD " with len %d: " FMT_WORD "\n", dtrace_array[i].map->name, dtrace_array[i].addr, dtrace_array[i].len, dtrace_array[i].read_data);
    }
    else
    {
        printf("device %s write in addr " FMT_WORD " with len %d: " FMT_WORD "->" FMT_WORD "\n", dtrace_array[i].map->name, dtrace_array[i].addr, dtrace_array[i].len, dtrace_array[i].read_data, dtrace_array[i].write_data);
    }
}

void print_dtrace()
{
    if (!dtrace_array_is_full && dtrace_array_tail == 0)
    {
        printf("dtrace is empty now\n");
        return;
    }
    if (dtrace_array_is_full)
    {
        int i = dtrace_array_tail;
        printf_dtrace_once(i);
        i++;
        if (i == DTRACE_ARRAY_MAX)
            i = 0;
        while (i != dtrace_array_tail)
        {
            printf_dtrace_once(i);
            i++;
            if (i == DTRACE_ARRAY_MAX)
                i = 0;
        }
    }
    else
    {
        int i = 0;
        while (i != dtrace_array_tail)
        {
            printf_dtrace_once(i);
            i++;
        }
    }
}

bool enable_device_fresh = true;
word_t map_read(paddr_t addr, int len, IOMap *map)
{
    assert(len >= 1 && len <= 8);
    check_bound(map, addr);
    paddr_t offset = addr - map->low;
    if (enable_device_fresh)
        invoke_callback(map->callback, offset, len, false); // prepare data to read
    word_t ret = host_read(map->space + offset, len);
#ifdef CONFIG_DTRACE
    dtrace_record(true, addr, len, map, ret, 0);
#endif
    return ret;
}

void map_write(paddr_t addr, int len, word_t data, IOMap *map)
{
    assert(len >= 1 && len <= 8);
    check_bound(map, addr);
    paddr_t offset = addr - map->low;
#ifdef CONFIG_DTRACE
    dtrace_record(false, addr, len, map, host_read(map->space + offset, len), data);
#endif
    host_write(map->space + offset, len, data);
    invoke_callback(map->callback, offset, len, true);
}
