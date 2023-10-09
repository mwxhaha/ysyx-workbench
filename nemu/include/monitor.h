#ifndef __MONITOR_H__
#define __MONITOR_H__

#include <common.h>
#include <stdbool.h>

word_t expr(const char *const e, bool *const success);

void test_expr();
void test_expr_auto();

int new_wp(const char *const e);
int free_wp(const int n);
bool check_watchpoint();
void printf_watchpoint();

#endif
