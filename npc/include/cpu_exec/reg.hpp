#ifndef REG_HPP
#define REG_HPP

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

void isa_reg_display();
word_t isa_reg_str2val(const char *s, bool *success);
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc);

#endif
