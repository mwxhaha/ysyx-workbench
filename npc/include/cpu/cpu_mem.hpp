#ifndef CPU_MEM_HPP
#define CPU_MEM_HPP

#include <cstdint>
#include <sim/cpu_sim.hpp>

#define MEM_BASE_ADDR 0x80000000
#define MEM_MAX 0x8000000

extern uint8_t mem[MEM_MAX];
void pmem_read(vaddr_t raddr, word_t *rdata);
void pmem_write(vaddr_t waddr, word_t wdata, uint8_t wmask);

#endif
