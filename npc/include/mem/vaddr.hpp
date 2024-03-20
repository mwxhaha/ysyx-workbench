#ifndef MEM_VADDR_HPP
#define MEM_VADDR_HPP

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

#define PAGE_SHIFT 12
#define PAGE_SIZE (1ul << PAGE_SHIFT)
#define PAGE_MASK (PAGE_SIZE - 1)

word_t addr_montior_read(vaddr_t addr, int len);
void addr_ifetch(vaddr_t addr, word_t *data);
void addr_read(vaddr_t addr, word_t *data);
void addr_read_with_clk(vaddr_t addr, word_t *data);
void addr_write(vaddr_t addr, uint8_t mask, word_t data);

#endif