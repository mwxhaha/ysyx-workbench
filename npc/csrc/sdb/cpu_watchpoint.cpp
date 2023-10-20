#include <sdb/cpu_watchpoint.hpp>

#include <cstdbool>
#include <cstdio>
#include <cstring>

#include <sim/cpu_sim.hpp>
#include <util/debug.hpp>
#include <sdb/cpu_expr.hpp>

#define NR_WP 32
#define WP_EXPR_MAX 32

typedef struct watchpoint
{
    bool used;
    char e[WP_EXPR_MAX + 1];
    word_t val;
} WP;

static WP wp_pool[NR_WP] = {};

void init_wp_pool()
{
    int i;
    for (i = 0; i < NR_WP; i++)
    {
        wp_pool[i].used = false;
        wp_pool[i].e[0] = '\0';
    }
}

int new_wp(const char *const e)
{
    int len = strlen(e);
    if (len >= WP_EXPR_MAX)
    {
        printf("expression is too long\n");
        return 1;
    }
    int i;
    for (i = 0; i < NR_WP; i++)
    {
        if (!wp_pool[i].used)
        {
            wp_pool[i].used = true;
            strcpy(wp_pool[i].e, e);
            bool success = true;
            wp_pool[i].val = expr(wp_pool[i].e, &success);
            if (!success)
            {
                printf("expression is illegal, can not set the watchpoint\n");
                wp_pool[i].used = false;
                return 1;
            }
            printf("successfully set the watchpoint, N: %d, expr: %s\n", i,
                   wp_pool[i].e);
            return 0;
        }
    }
    printf("there is no free watchpoint\n");
    return 1;
}

int free_wp(const int n)
{
    if (n <= -1 || n >= NR_WP)
    {
        printf("the N is out of range\n");
        return 1;
    }
    if (!wp_pool[n].used)
    {
        printf("the watchpoint is orignally free\n");
        return 1;
    }
    printf("successfully delete the watchpoint, N: %d, expr: %s\n", n,
           wp_pool[n].e);
    wp_pool[n].used = false;
    wp_pool[n].e[0] = '\0';
    return 0;
}

bool check_watchpoint()
{
    bool stop_flag = false;
    for (int i = 0; i < NR_WP; i++)
        if (wp_pool[i].used)
        {
            bool success = true;
            word_t new_val = expr(wp_pool[i].e, &success);
            if (!success)
                panic("The expression was illegally modified");
            if (wp_pool[i].val != new_val)
            {
                printf("watchpoint changes, N: %d, expr: %s, value: " FMT_WORD
                       " -> " FMT_WORD "\n",
                       i, wp_pool[i].e, wp_pool[i].val, new_val);
                stop_flag = true;
            }
            wp_pool[i].val = new_val;
        }
    return stop_flag;
}

void printf_watchpoint()
{
    printf("watchpoint                             expr      value\n");
    bool empty_flag = true;
    for (int i = 0; i < NR_WP; i++)
        if (wp_pool[i].used)
        {
            printf("%10d %32s " FMT_WORD "\n", i, wp_pool[i].e, wp_pool[i].val);
            empty_flag = false;
        }
    if (empty_flag)
        printf("watchpoint pool is empty\n");
}
