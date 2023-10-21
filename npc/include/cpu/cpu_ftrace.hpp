#ifndef CPU_FTRACE_HPP
#define CPU_FTRACE_HPP

#include <cpu/cpu_cpu_exec.hpp>

void load_elf(const char *elf_file);
void ftrace_record(Decode *s);
void print_ftrace();

#endif
