#ifndef CPU_WATCHPOINT_HPP
#define CPU_WATCHPOINT_HPP

void init_wp_pool();
int new_wp(const char *const e);
int free_wp(const int n);
bool check_watchpoint();
void printf_watchpoint();

#endif