#ifndef CPU_EXPR_HPP
#define CPU_EXPR_HPP

#include <sim_tool.hpp>

void init_regex();
word_t expr(const char *const e, bool *const success);
void test_expr();
void test_expr_auto();

#endif