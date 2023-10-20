#ifndef CPU_REG_HPP
#define CPU_REG_HPP

#include <cstdbool>

#include <sim/cpu_sim.hpp>

void isa_reg_display();
word_t isa_reg_str2val(const char *s, bool *success);

#endif
