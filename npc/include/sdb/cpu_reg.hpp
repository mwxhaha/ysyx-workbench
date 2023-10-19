#ifndef CPU_REG_HPP
#define CPU_REG_HPP

#include <util/sim_tool.hpp>
#include <sim/cpu_sim.hpp>

void isa_reg_display();
word_t isa_reg_str2val(const char *s, bool *success);

#endif