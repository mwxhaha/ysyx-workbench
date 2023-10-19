#ifndef CPU_CPU_EXEC_HPP
#define CPU_CPU_EXEC_HPP

#include <cstdint>
#include <util/sim_tool.hpp>
#include <sim/cpu_sim.hpp>

typedef struct {
  union {
    uint32_t val;
  } inst;
} ISADecodeInfo;

typedef struct Decode {
  vaddr_t pc;
  vaddr_t snpc; // static next pc
  vaddr_t dnpc; // dynamic next pc
  ISADecodeInfo isa;
  char logbuf[128];
} Decode;

void assert_fail_msg();
void cpu_exec(uint64_t n);

#endif