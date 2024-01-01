#ifndef __MONITOR_H__
#define __MONITOR_H__

#include <common.h>
#include <stdbool.h>

word_t expr(const char *const e, bool *const success);
void test_expr();
void test_expr_auto();

void new_wp(int hit_cnt, const char *const e);
void free_wp(const int n);
bool check_watchpoint();
void printf_watchpoint();

#ifndef CONFIG_TARGET_AM
void init_monitor(int argc, char *argv[]);
void monitor_quit();
#else
void am_init_monitor();
void am_monitor_quit();
#endif

#endif
