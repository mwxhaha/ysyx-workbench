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

#include <device/map.h>
#include <memory/paddr.h>

#define NR_MAP 16

static IOMap maps[NR_MAP] = {};
static int nr_map = 0;

static IOMap *fetch_mmio_map(paddr_t addr)
{
    int mapid = find_mapid_by_addr(maps, nr_map, addr);
    return (mapid == -1 ? NULL : &maps[mapid]);
}

static void report_mmio_overlap(const char *name1, paddr_t l1, paddr_t r1,
                                const char *name2, paddr_t l2, paddr_t r2)
{
    panic("MMIO region %s@[" FMT_PADDR ", " FMT_PADDR "] is overlapped "
          "with %s@[" FMT_PADDR ", " FMT_PADDR "]",
          name1, l1, r1, name2, l2, r2);
}

/* device interface */
void add_mmio_map(const char *name, paddr_t addr, void *space, uint32_t len, io_callback_t callback)
{
    assert(nr_map < NR_MAP);
    paddr_t left = addr, right = addr + len - 1;
    if (in_pmem(left) || in_pmem(right))
    {
        report_mmio_overlap(name, left, right, "pmem", PMEM_LEFT, PMEM_RIGHT);
    }
    for (int i = 0; i < nr_map; i++)
    {
        if (left <= maps[i].high && right >= maps[i].low)
        {
            report_mmio_overlap(name, left, right, maps[i].name, maps[i].low, maps[i].high);
        }
    }

    maps[nr_map] = (IOMap){.name = name, .low = addr, .high = addr + len - 1, .space = space, .callback = callback};
    Log("Add mmio map '%s' at [" FMT_PADDR ", " FMT_PADDR "]",
        maps[nr_map].name, maps[nr_map].low, maps[nr_map].high);

    nr_map++;
}

typedef struct
{
    bool is_read;
    paddr_t addr;
    int len;
    IOMap *map;
    word_t data;
} dtrace_t;
#define DTRACE_ARRAY_MAX 20
static dtrace_t dtrace_array[DTRACE_ARRAY_MAX];
static int dtrace_array_tail = 0;
static bool dtrace_array_is_full = false;

static void dtrace_record(bool is_read, paddr_t addr, int len, IOMap *map, word_t data)
{
    if (addr >= 0x8fffffff && addr <= 0xffffffff)
    {
        dtrace_array[dtrace_array_tail].is_read = is_read;
        dtrace_array[dtrace_array_tail].addr = addr;
        dtrace_array[dtrace_array_tail].len = len;
        dtrace_array[dtrace_array_tail].data = data;
        dtrace_array[dtrace_array_tail].map = map;
        dtrace_array_tail++;
        if (dtrace_array_tail >= DTRACE_ARRAY_MAX)
        {
            dtrace_array_tail = 0;
            dtrace_array_is_full = true;
        }
    }
}

static void printf_dtrace_once(int i)
{
    if (dtrace_array[i].is_read)
    {
        printf("device %s read in addr " FMT_WORD " with len %d: " FMT_WORD "\n", dtrace_array[i].map->name, dtrace_array[i].addr, dtrace_array[i].len, dtrace_array[i].data);
    }
    else
    {
        printf("device %s write in addr " FMT_WORD " with len %d: " FMT_WORD "\n", dtrace_array[i].map->name, dtrace_array[i].addr, dtrace_array[i].len, dtrace_array[i].data);
    }
}

void print_dtrace()
{
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

/* bus interface */
word_t mmio_read(paddr_t addr, int len)
{
    IOMap *map = fetch_mmio_map(addr);
    word_t ret = map_read(addr, len, map);
#ifdef CONFIG_DTRACE
    dtrace_record(true, addr, len, map, ret);
#endif
    return ret;
}

void mmio_write(paddr_t addr, int len, word_t data)
{
    IOMap *map = fetch_mmio_map(addr);
    map_write(addr, len, data, map);
#ifdef CONFIG_DTRACE
    dtrace_record(false, addr, len, map, data);
#endif
}
