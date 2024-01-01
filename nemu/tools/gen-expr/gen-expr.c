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
static char buf[BUF_MAX] = {};
static int buf_i = 0;
static char code_buf[BUF_MAX + 128] = {}; // a little larger than `buf`
static char *code_format =
#ifdef CONFIG_ISA64
    "#include <stdio.h>\n"
    "#include <stdint.h>\n"
    "int main() { "
    "  uint64_t result = %s; "
    "  printf(\"%%lu\", result); "
    "  return 0; "
    "}";
#else
    "#include <stdio.h>\n"
    "#include <stdint.h>\n"
    "int main() { "
    "  uint32_t result = %s; "
    "  printf(\"%%u\", result); "
    "  return 0; "
    "}";
#endif

static int choose(int n) { return rand() % n; }

static bool gen_num()
{
    word_t number = rand() % 10000;
    sprintf(buf + buf_i, FMT_WORD_T, number);
    if (number != 0)
        buf_i += (int)log10(number) + 1;
    else
        buf_i++;
    if (buf_i >= BUF_MAX)
        return false;
    return true;
}

static bool gen(char c)
{
    sprintf(buf + buf_i, "%c", c);
    buf_i++;
    if (buf_i >= BUF_MAX)
        return false;
    return true;
}

static bool gen_rand_op()
{
    switch (choose(4))
    {
    case 0:
        sprintf(buf + buf_i, "%c", '+');
        break;
    case 1:
        sprintf(buf + buf_i, "%c", '-');
        break;
    case 2:
        sprintf(buf + buf_i, "%c", '*');
        break;
    default:
        sprintf(buf + buf_i, "%c", '/');
        break;
    }
    buf_i++;
    if (buf_i >= BUF_MAX)
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

    FILE *fp2 =
        fopen("/home/mwxhaha/ysyx-workbench/nemu/tools/gen-expr/input", "w");
    assert(fp2 != NULL);
    for (int i = 0; i < loop; i++)
    {
        buf_i = 0;
        if (!gen_rand_expr())
            continue;
        buf[buf_i] = '\0';
        sprintf(code_buf, code_format, buf);

        FILE *fp = fopen("/tmp/.code.c", "w");
        assert(fp != NULL);
        fputs(code_buf, fp);
        fclose(fp);

        int ret = system("gcc /tmp/.code.c -Werror -o /tmp/.expr");
        if (ret != 0)
            continue;
        fp = popen("/tmp/.expr", "r");
        assert(fp != NULL);
        word_t result;
        ret = fscanf(fp, FMT_WORD_T, &result);
        if (ret != 1)
            continue;
        pclose(fp);

        while (buf_i >= 0)
        {
#ifdef CONFIG_ISA64
            if (buf[buf_i] == 'l')
                buf[buf_i] = ' ';
#endif
            if (buf[buf_i] == 'u')
                buf[buf_i] = ' ';
            buf_i--;
        }
        fprintf(fp2, FMT_WORD_T "\n%s\n", result, buf);
    }
    fclose(fp2);
    return 0;
}
