#ifndef CPU_EXEC_DUT_HPP
#define CPU_EXEC_DUT_HPP

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

void difftest_skip_ref();
void difftest_skip_dut(int nr_ref, int nr_dut);
void init_difftest(char *ref_so_file, long img_size);
void difftest_step(vaddr_t pc, vaddr_t npc);

#endif
