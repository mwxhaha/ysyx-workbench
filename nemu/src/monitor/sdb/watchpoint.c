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
#include <debug.h>
#include <monitor.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define NR_WP 32
#define WP_EXPR_MAX 100

typedef struct watchpoint
{
    int hit_cnt;
    char e[WP_EXPR_MAX + 1];
    word_t val;
} WP;

static WP wp_pool[NR_WP] = {};

void init_wp_pool()
{
    int i;
    for (i = 0; i < NR_WP; i++)
    {
        wp_pool[i].hit_cnt = 0;
        wp_pool[i].e[0] = '\0';
    }
}

void new_wp(int hit_cnt, const char *const e)
{
    int len = strlen(e);
    if (len >= WP_EXPR_MAX)
    {
        printf("expression is too long\n");
        return;
    }
    if (hit_cnt <= 0)
    {
        printf("hit cnt must be positive integer\n");
    }
    int i;
    for (i = 0; i < NR_WP; i++)
    {
        if (wp_pool[i].hit_cnt == 0)
        {
            bool success = true;
            wp_pool[i].val = expr(e, &success);
            if (!success)
            {
                printf("expression is illegal, can not set the watchpoint\n");
                return;
            }
            wp_pool[i].hit_cnt = hit_cnt;
            strcpy(wp_pool[i].e, e);
            printf("successfully set the watchpoint, N: %d, hit cnt: %d, expr: %s, val: %d\n", i, wp_pool[i].hit_cnt, wp_pool[i].e, wp_pool[i].val);
            return;
        }
    }
    printf("there is no free watchpoint\n");
}

void free_wp(const int n)
{
    if (n <= -1 || n >= NR_WP)
    {
        printf("the N is out of range\n");
        return;
    }
    if (wp_pool[n].hit_cnt == 0)
    {
        printf("the watchpoint is orignally free\n");
        return;
    }
    printf("successfully delete the watchpoint, N: %d, remaining hit cnt: %d, expr: %s, val: %d\n", n, wp_pool[n].hit_cnt, wp_pool[n].e, wp_pool[n].val);
    wp_pool[n].hit_cnt = 0;
    wp_pool[n].e[0] = '\0';
}

bool check_watchpoint()
{
    bool stop_flag = false;
    for (int i = 0; i < NR_WP; i++)
        if (wp_pool[i].hit_cnt > 0)
        {
            bool success = true;
            word_t new_val = expr(wp_pool[i].e, &success);
            if (!success)
                panic("The expression was illegally modified");
            if (wp_pool[i].val != new_val)
            {
                wp_pool[i].hit_cnt--;
                if (wp_pool[i].hit_cnt == 0)
                {
                    stop_flag = true;
                    printf("watchpoint changes, N: %d, expr: %s, value: " FMT_WORD " -> " FMT_WORD "\n", i, wp_pool[i].e, wp_pool[i].val, new_val);
                }
            }
            wp_pool[i].val = new_val;
        }
        else if (wp_pool[i].hit_cnt < 0)
            panic("The watchpoint cnt error");
    return stop_flag;
}

void printf_watchpoint()
{
    printf("index   cnt                             expr      value\n");
    bool empty_flag = true;
    for (int i = 0; i < NR_WP; i++)
        if (wp_pool[i].hit_cnt > 0)
        {
            printf("%5d %5d %32s " FMT_WORD "\n", i, wp_pool[i].hit_cnt, wp_pool[i].e, wp_pool[i].val);
            empty_flag = false;
        }
    if (empty_flag)
        printf("watchpoint pool is empty\n");
}
