/***************************************************************************************
 * Copyright (c) 2014-2022 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan
 *PSL v2. You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
 *KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 *NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include <common.h>
#include <monitor.h>
void engine_start();
int is_exit_status_bad();

#define TEST_EXPR

int main(int argc, char *argv[])
{
    /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
    am_init_monitor();
#else
    init_monitor(argc, argv);
#endif

    /* Start engine. */
#ifndef TEST_EXPR
    engine_start();
#else
    test_expr();
    // test_expr_auto();
#endif

#ifdef CONFIG_TARGET_AM
    am_monitor_quit();
#else
    monitor_quit();
#endif

#ifndef TEST_EXPR
    return is_exit_status_bad();
#else
    return 0;
#endif
}
