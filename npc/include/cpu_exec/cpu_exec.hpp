#ifndef CPU_EXEC_CPU_EXEC_HPP
#define CPU_EXEC_CPU_EXEC_HPP

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

typedef struct
{
    union
    {
        uint32_t val;
    } inst;
} ISADecodeInfo;

typedef struct Decode
{
    vaddr_t pc;
    vaddr_t snpc; // static next pc
    vaddr_t dnpc; // dynamic next pc
    ISADecodeInfo isa;
    char logbuf[128];
} Decode;

extern uint64_t g_nr_guest_inst;

void assert_fail_msg();
void cpu_exec(uint64_t n);

#endif
