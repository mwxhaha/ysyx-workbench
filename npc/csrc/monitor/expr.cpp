#include <monitor/expr.hpp>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <regex.h>

#include <sim/cpu.hpp>
#include <util/debug.hpp>
#include <util/macro.hpp>
#include <util/sim_tool.hpp>
#include <cpu_exec/log.hpp>
#include <cpu_exec/mem.hpp>
#include <cpu_exec/reg.hpp>

enum
{
    TK_NOTYPE = 256,
    TK_NUMBER,
    TK_NUMBER_HEX,
    TK_REG,
    TK_NEG,
    TK_DEREF,
    TK_EQ,
    TK_NE,
    TK_GE,
    TK_LE,
    TK_AND,
    TK_OR,
    TK_SL,
    TK_SR,
};

static struct rule
{
    const char *regex;
    int token_type;
} rules[] = {
    {" +", TK_NOTYPE},
    {"0x[0-9a-fA-F]+", TK_NUMBER_HEX},
    {"\\$[0-9a-zA-Z\\$]+", TK_REG},
    {"[0-9]+", TK_NUMBER},
    {"\\+", '+'},
    {"-", '-'},
    {"\\*", '*'},
    {"/", '/'},
    {"\\(", '('},
    {"\\)", ')'},
    {"==", TK_EQ},
    {"!=", TK_NE},
    {">=", TK_GE},
    {"<=", TK_LE},
    {"<<", TK_SL},
    {">>", TK_SR},
    {">", '>'},
    {"<", '<'},
    {"&&", TK_AND},
    {"\\|\\|", TK_OR},
    {"!", '!'},
    {"&", '&'},
    {"\\|", '|'},
    {"\\^", '^'},
    {"~", '~'},
};

#define NR_REGEX ARRLEN(rules)

static int operator_precedence[300];

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; i++)
    {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0)
        {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }

    operator_precedence[TK_NEG] = 2;
    operator_precedence[TK_DEREF] = 2;
    operator_precedence['!'] = 2;
    operator_precedence['~'] = 2;
    operator_precedence['*'] = 3;
    operator_precedence['/'] = 3;
    operator_precedence['+'] = 4;
    operator_precedence['-'] = 4;
    operator_precedence[TK_SL] = 5;
    operator_precedence[TK_SR] = 5;
    operator_precedence['>'] = 6;
    operator_precedence['<'] = 6;
    operator_precedence[TK_GE] = 6;
    operator_precedence[TK_LE] = 6;
    operator_precedence[TK_EQ] = 7;
    operator_precedence[TK_NE] = 7;
    operator_precedence['&'] = 8;
    operator_precedence['^'] = 9;
    operator_precedence['|'] = 10;
    operator_precedence[TK_AND] = 11;
    operator_precedence[TK_OR] = 12;
}

#define TOKEN_STR_MAX 30

typedef struct token
{
    int type;
    char str[TOKEN_STR_MAX];
} Token;

#define TOKENS_MAX 65536
static Token tokens[TOKENS_MAX] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;

static bool make_token(const char *const e)
{
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_token = 0;

    while (e[position] != '\0')
    {
        /* Try all rules one by one. */
        for (i = 0; i < NR_REGEX; i++)
        {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 &&
                pmatch.rm_so == 0)
            {
                const char *const substr_start = e + position;
                int substr_len = pmatch.rm_eo;

#ifdef CONFIG_EXPR_MATCH
                printf("match rules[%d] = \"%s\" at position %d with len %d: %.*s\n",
                       i, rules[i].regex, position, substr_len, substr_len, substr_start);
#endif

                position += substr_len;

                switch (rules[i].token_type)
                {
                case TK_NOTYPE:
                    break;
                case TK_NUMBER:
                case TK_NUMBER_HEX:
                case TK_REG:
                    Assert(substr_len <= sizeof(word_t) * 8, "number is too long");
                    Assert(memcpy(tokens[nr_token].str, substr_start, substr_len), "string process error");
                    tokens[nr_token].str[substr_len] = '\0';
                    tokens[nr_token].type = rules[i].token_type;
                    nr_token++;
                    break;
                default:
                    tokens[nr_token].type = rules[i].token_type;
                    nr_token++;
                    break;
                }
                break;
            }
        }

        if (i == NR_REGEX)
        {
            Log("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }
    for (i = 0; i < nr_token; i++)
    {
        if (tokens[i].type == '*')
            if (i == 0 ||
                !(tokens[i - 1].type == TK_NUMBER || tokens[i - 1].type == TK_NUMBER_HEX ||
                  tokens[i - 1].type == TK_REG || tokens[i - 1].type == ')'))
            {
                tokens[i].type = TK_DEREF;
            }
        if (tokens[i].type == '-')
            if (i == 0 ||
                !(tokens[i - 1].type == TK_NUMBER || tokens[i - 1].type == TK_NUMBER_HEX ||
                  tokens[i - 1].type == TK_REG || tokens[i - 1].type == ')'))
            {
                tokens[i].type = TK_NEG;
            }
    }

    return true;
}

static int find_parentheses(int p, bool *const success)
{
    if (tokens[p].type != '(')
    {
        *success = false;
        printf("brackets does not match\n");
        return -1;
    }

    int cnt = 1;
    p++;
    while (p < nr_token && cnt != 0)
    {
        if (tokens[p].type == '(')
            cnt++;
        else if (tokens[p].type == ')')
            cnt--;
        p++;
    }
    if (p == nr_token && cnt != 0)
    {
        *success = false;
        printf("brackets does not match\n");
        return -1;
    }
    return p - 1;
}

static bool check_parentheses(const int p, const int q, bool *const success)
{
    if (tokens[p].type != '(' || tokens[q].type != ')')
        return false;
    int q_match = find_parentheses(p, success);
    if (!(*success))
        return false;

    if (q_match == q)
        return true;
    else if (q_match < q)
        return false;
    else
    {
        *success = false;
        printf("brackets does not match\n");
        return false;
    }
}

static bool compare_operator_precedence(int operator1, int operator2)
{
    return operator_precedence[operator1] <= operator_precedence[operator2];
}

static int find_main_op(int p, const int q, bool *const success)
{
    int main_op = -1;
    while (p <= q)
    {
        switch (tokens[p].type)
        {
        case '+':
        case '-':
        case '*':
        case '/':
        case TK_NEG:
        case TK_DEREF:
        case TK_EQ:
        case TK_NE:
        case '>':
        case '<':
        case TK_GE:
        case TK_LE:
        case TK_AND:
        case TK_OR:
        case '!':
        case '&':
        case '|':
        case '^':
        case '~':
        case TK_SL:
        case TK_SR:
            if (main_op == -1)
                main_op = p;
            else if (compare_operator_precedence(tokens[main_op].type, tokens[p].type) &&
                     !(operator_precedence[tokens[main_op].type] == 2 &&
                       operator_precedence[tokens[p].type] == 2))
                main_op = p;
            p++;
            break;
        case '(':
            p = find_parentheses(p, success) + 1;
            if (!(*success))
                return -1;
            break;
        default:
            p++;
            break;
        }
    }
    return main_op;
}

static word_t eval(const int p, const int q, bool *const success)
{
    if (p > q)
    {
        *success = false;
        printf("expression split failed\n");
        return -1;
    }
    else if (p == q)
    {
        word_t number;
        bool success_tmp = true;
        switch (tokens[p].type)
        {
        case TK_NUMBER:
            if (sscanf(tokens[p].str, FMT_WORD_T, &number) == 1)
            {
                return number;
            }
            else
            {
                *success = false;
                printf("the number in expression is illegal\n");
                return -1;
            }
            break;
        case TK_NUMBER_HEX:
            if (sscanf(tokens[p].str, "0x%x", &number) == 1)
            {
                return number;
            }
            else
            {
                *success = false;
                printf("the number in expression is illegal\n");
                return -1;
            }
            break;
        case TK_REG:
            number = isa_reg_str2val(tokens[p].str + 1, &success_tmp);
            if (!success_tmp)
            {
                *success = false;
                printf("the register does not exist\n");
                return -1;
            }
            else
            {
                return number;
            }
            break;
        default:
            *success = false;
            printf("expression split failed\n");
            return -1;
            break;
        }
    }
    else
    {
        bool check = check_parentheses(p, q, success);
        if (!(*success))
            return -1;
        if (check)
        {
            word_t val = eval(p + 1, q - 1, success);
            if (!(*success))
                return -1;
            return val;
        }
        else
        {
            int main_op = find_main_op(p, q, success);
            if (!(*success))
                return -1;
            word_t val1 = 0;
            if (operator_precedence[tokens[main_op].type] != 2)
            {
                val1 = eval(p, main_op - 1, success);
                if (!(*success))
                    return -1;
            }
            word_t val2 = eval(main_op + 1, q, success);
            if (!(*success))
                return -1;
            if (tokens[main_op].type == '/' && val2 == 0)
            {
                *success = false;
                printf("do not div 0\n");
                return -1;
            }
            switch (tokens[main_op].type)
            {
            case '+':
                return val1 + val2;
            case '-':
                return val1 - val2;
            case '*':
                return val1 * val2;
            case '/':
                return val1 / val2;
            case TK_NEG:
                return -val2;
            case TK_DEREF:
                IFDEF(CONFIG_MTRACE, disable_mtrace_once());
                return pmem_read(val2, sizeof(word_t));
            case TK_EQ:
                return val1 == val2;
            case TK_NE:
                return val1 != val2;
            case '>':
                return val1 > val2;
            case '<':
                return val1 < val2;
            case TK_GE:
                return val1 >= val2;
            case TK_LE:
                return val1 <= val2;
            case TK_AND:
                return val1 && val2;
            case TK_OR:
                return val1 || val2;
            case '!':
                return !val2;
            case '&':
                return val1 & val2;
            case '|':
                return val1 | val2;
            case '^':
                return val1 ^ val2;
            case '~':
                return ~val2;
            case TK_SL:
                return val1 << val2;
            case TK_SR:
                return val1 >> val2;
            default:
                *success = false;
                printf("operator analysis failed\n");
                return -1;
            }
        }
    }
}

word_t expr(const char *const e, bool *const success)
{
    if (!make_token(e))
    {
        *success = false;
        return -1;
    }

    word_t val = eval(0, nr_token - 1, success);
    if (!(*success))
        return -1;

    return val;
}

void test_expr()
{
    bool success = true;
    word_t val = expr("  -*0x80000010", &success);
    Assert(success, "expression is illegal");
    printf(FMT_WORD_T "\n", val);
}

static char expr_str[TOKENS_MAX];

void test_expr_auto()
{
    FILE *fp = fopen("/home/mwxhaha/ysyx-workbench/nemu/tools/gen-expr/build/test_expr.txt", "r");
    Assert(fp, "file does not exist");
    while (1)
    {
        word_t real_result;
        int ret = fscanf(fp, FMT_WORD_T "\n", &real_result);
        if (ret == EOF)
            break;
        Assert(ret == 1, "read file error");
        char *ret2 = fgets(expr_str, TOKENS_MAX, fp);
        Assert(ret2, "read file error");
        expr_str[strlen(expr_str) - 1] = '\0';

        bool success = true;
        word_t cal_result = expr(expr_str, &success);
        Assert(success, "expression is illegal");
        printf(FMT_WORD_T " == " FMT_WORD_T "\n%s\n", real_result, cal_result, expr_str);
        Assert(real_result == cal_result, "wrong answer");
    }
    fclose(fp);
}
