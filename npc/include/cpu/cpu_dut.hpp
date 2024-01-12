#ifndef CPU_DUT_HPP
#define CPU_DUT_HPP

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include <sim/cpu_sim.hpp>
#include <util/debug.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>

void init_difftest(char *ref_so_file, long img_size);
void difftest_step(vaddr_t pc, vaddr_t npc);

#endif
