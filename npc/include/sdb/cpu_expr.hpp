#ifndef CPU_EXPR_HPP
#define CPU_EXPR_HPP

#include <util/sim_tool.hpp>
#include <sim/cpu_sim.hpp>

void init_regex();
word_t expr(const char *const e, bool *const success);
void test_expr();
void test_expr_auto();

#endif