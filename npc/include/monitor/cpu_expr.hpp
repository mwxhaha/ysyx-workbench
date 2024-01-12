#ifndef CPU_EXPR_HPP
#define CPU_EXPR_HPP

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

void init_regex();
word_t expr(const char *const e, bool *const success);
void test_expr();
void test_expr_auto();

#endif
