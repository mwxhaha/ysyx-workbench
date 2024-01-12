#ifndef CPU_MEM_HPP
#define CPU_MEM_HPP

#include <cstdint>
#include <sim/cpu_sim.hpp>

#define MEM_BASE_ADDR 0x80000000
#define MEM_MAX 0x8000000

extern uint8_t pmem[MEM_MAX];

void print_mtrace();
void disable_mtrace_once();
word_t pmem_read(paddr_t addr, int len);
void pmem_write(paddr_t addr, int len, word_t data);

#endif
