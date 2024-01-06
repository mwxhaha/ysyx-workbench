#ifndef CPU_WATCHPOINT_HPP
#define CPU_WATCHPOINT_HPP

#include <cstdbool>

void init_wp_pool();
void new_wp(int hit_cnt, const char *const e);
void free_wp(const int n);
bool check_watchpoint();
void printf_watchpoint();

#endif
