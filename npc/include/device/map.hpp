#ifndef MAP_HPP
#define MAP_HPP

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

typedef void (*io_callback_t)(uint32_t, int, bool);
typedef struct
{
    const char *name;
    // we treat ioaddr_t as paddr_t here
    paddr_t low;
    paddr_t high;
    void *space;
    io_callback_t callback;
} IOMap;

int find_mapid_by_addr(IOMap *maps, int size, paddr_t addr);
uint8_t *new_space(int size);
void init_map();
void map_quit();
void print_dtrace();
word_t map_read(paddr_t addr, int len, IOMap *map);
void map_write(paddr_t addr, int len, word_t data, IOMap *map);

#endif
