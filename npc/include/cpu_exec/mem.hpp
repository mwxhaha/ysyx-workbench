#ifndef MEM_HPP
#define MEM_HPP

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

#define MEM_BASE_ADDR 0x80000000
#define MEM_MAX 0x8000000

extern uint8_t pmem[MEM_MAX];

void print_mtrace();
void disable_mtrace_once();
word_t pmem_read(paddr_t addr, int len);
void pmem_write(paddr_t addr, int len, word_t data);

#endif
