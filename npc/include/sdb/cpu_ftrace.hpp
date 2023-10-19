#ifndef CPU_FTRACE_HPP
#define CPU_FTRACE_HPP

#include <cpu_cpu_exec.hpp>
#include <cstdbool>

bool load_elf(int argc, char **argv);
void ftrace_record(Decode *s);
void print_ftrace();

#endif