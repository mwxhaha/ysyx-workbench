#ifndef FTRACE_HPP
#define FTRACE_HPP

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
#include <cpu_exec/cpu_exec.hpp>

void load_elf(const char *elf_file);
void ftrace_record(Decode *s);
void print_ftrace();

#endif
