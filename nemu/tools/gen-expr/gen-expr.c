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

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// this should be enough
#define BUF_MAX 65536
// #define CONFIG_ISA64
#ifdef CONFIG_ISA64
typedef uint64_t word_t;
#define FMT_WORD_T "%lu"
#else
typedef uint32_t word_t;
#define FMT_WORD_T "%u"
#endif

static char expr_str[BUF_MAX] = {};
static int expr_str_tail = 0;
static char code[BUF_MAX + 128] = {}; // a little larger than `expr_str`
static char *code_format =
    "#include <stdio.h>\n"
    "#include <stdint.h>\n"
    "int main() { "
#ifdef CONFIG_ISA64
    "  uint64_t real_result = %s; "
    "  printf(\"%%lu\", real_result); "
#else
    "  uint32_t real_result = %s; "
    "  printf(\"%%u\", real_result); "
#endif
    "  return 0; "
    "}";

#define PATH "/home/mwxhaha/ysyx-workbench/nemu/tools/gen-expr"

static int choose(int n) { return rand() % n; }

static bool gen_num()
{
    word_t number = rand() % 10000;
    sprintf(expr_str + expr_str_tail, FMT_WORD_T, number);
    if (number != 0)
        expr_str_tail += (int)log10(number) + 1;
    else
        expr_str_tail++;
    if (expr_str_tail >= BUF_MAX)
        return false;
    return true;
}

static bool gen(char c)
{
    sprintf(expr_str + expr_str_tail, "%c", c);
    expr_str_tail++;
    if (expr_str_tail >= BUF_MAX)
        return false;
    return true;
}

static bool gen_rand_op()
{
    switch (choose(4))
    {
    case 0:
        sprintf(expr_str + expr_str_tail, "%c", '+');
        break;
    case 1:
        sprintf(expr_str + expr_str_tail, "%c", '-');
        break;
    case 2:
        sprintf(expr_str + expr_str_tail, "%c", '*');
        break;
    default:
        sprintf(expr_str + expr_str_tail, "%c", '/');
        break;
    }
    expr_str_tail++;
    if (expr_str_tail >= BUF_MAX)
        return false;
    return true;
}

static bool gen_empty()
{
    int n = rand() % 3;
    for (int i = 1; i <= n; i++)
    {
        if (!gen(' '))
            return false;
    }
    return true;
}

static bool gen_rand_expr()
{
    switch (choose(3))
    {
    case 0:
        if (!gen_empty())
            return false;
        if (!gen_num())
            return false;
        if (!gen('u'))
            return false;
#ifdef CONFIG_ISA64
        if (!gen('l'))
            return false;
#endif
        if (!gen_empty())
            return false;
        break;
    case 1:
        if (!gen('('))
            return false;
        if (!gen_empty())
            return false;
        if (!gen_rand_expr())
            return false;
        if (!gen_empty())
            return false;
        if (!gen(')'))
            return false;
        break;
    default:
        if (!gen_rand_expr())
            return false;
        if (!gen_empty())
            return false;
        if (!gen_rand_op())
            return false;
        if (!gen_empty())
            return false;
        if (!gen_rand_expr())
            return false;
        break;
    }
    return true;
}

int main(int argc, char *argv[])
{
    int seed = time(0);
    srand(seed);
    int loop = 100;
    if (argc > 1)
    {
        sscanf(argv[1], "%d", &loop);
    }

    FILE *fp2 = fopen(PATH "/build/test_expr.txt", "w");
    assert(fp2 != NULL);
    for (int i = 0; i < loop; i++)
    {
        expr_str_tail = 0;
        if (!gen_rand_expr())
            continue;
        expr_str[expr_str_tail] = '\0';
        sprintf(code, code_format, expr_str);

        FILE *fp = fopen(PATH "/build/.code.c", "w");
        assert(fp != NULL);
        fputs(code, fp);
        fclose(fp);

        int ret = system("gcc " PATH "/build/.code.c -Werror -o " PATH "/build/.code");
        if (ret != 0)
            continue;
        fp = popen(PATH "/build/.code", "r");
        assert(fp != NULL);
        word_t real_result;
        ret = fscanf(fp, FMT_WORD_T, &real_result);
        if (ret != 1)
            continue;
        pclose(fp);

        while (expr_str_tail >= 0)
        {
#ifdef CONFIG_ISA64
            if (expr_str[expr_str_tail] == 'l')
                expr_str[expr_str_tail] = ' ';
#endif
            if (expr_str[expr_str_tail] == 'u')
                expr_str[expr_str_tail] = ' ';
            expr_str_tail--;
        }
        fprintf(fp2, FMT_WORD_T "\n%s\n", real_result, expr_str);
    }
    fclose(fp2);
    return 0;
}
