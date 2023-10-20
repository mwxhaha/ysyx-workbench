#ifndef CPU_FTRACE_HPP
#define CPU_FTRACE_HPP

#include <cstdbool>

#include <cpu_cpu_exec.hpp>

bool load_elf(int argc, char **argv);
void ftrace_record(Decode *s);
void print_ftrace();

#endif
