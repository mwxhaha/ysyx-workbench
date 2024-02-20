#ifndef WATCHPOINT_HPP
#define WATCHPOINT_HPP

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

void init_wp_pool();
void new_wp(int hit_cnt, const char *const e);
void free_wp(const int n);
bool check_watchpoint();
void printf_watchpoint();

#endif
