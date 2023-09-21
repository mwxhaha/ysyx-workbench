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
#include <isa.h>
#include <memory/vaddr.h>
#include <regex.h>
#include <stdbool.h>
#include <string.h>

enum {
  TK_NOTYPE = 256,
  TK_NUMBER,
  TK_EQ,
  TK_NEQ,
  TK_AND,
  TK_NUMBER_HEX,
  TK_REG,
  TK_DEREF,
  TK_MINUS_ONE
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {{" +", TK_NOTYPE},
             {"0x[0-9a-fA-F]+", TK_NUMBER_HEX},
             {"[0-9]+", TK_NUMBER},
             {"\\+", '+'},
             {"-", '-'},
             {"\\*", '*'},
             {"/", '/'},
             {"\\(", '('},
             {"\\)", ')'},
             {"==", TK_EQ},
             {"!=", TK_NEQ},
             {"&&", TK_AND},
             {"\\$[0-9a-zA-Z\\$]+", TK_REG}};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[sizeof(word_t) * 8 + 1];
} Token;

#define TOKENS_MAX 65536
static Token tokens[TOKENS_MAX] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;

static bool make_token(const char *const e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 &&
          pmatch.rm_so == 0) {
        const char *const substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i,
        //     rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case TK_NUMBER:
          case TK_NUMBER_HEX:
          case TK_REG:
            Assert(substr_len <= sizeof(word_t) * 8, "number is too long");
            Assert(memcpy(tokens[nr_token].str, substr_start, substr_len),
                   "string process error");
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

    if (i == NR_REGEX) {
      Log("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  for (i = 0; i < nr_token; i++) {
    if (tokens[i].type == '*')
      if (i == 0 || tokens[i - 1].type == '+' || tokens[i - 1].type == '-' ||
          tokens[i - 1].type == '*' || tokens[i - 1].type == '/' ||
          tokens[i - 1].type == '(' || tokens[i - 1].type == TK_EQ ||
          tokens[i - 1].type == TK_NEQ || tokens[i - 1].type == TK_AND ||
          tokens[i - 1].type == TK_DEREF ||
          tokens[i - 1].type == TK_MINUS_ONE) {
        tokens[i].type = TK_DEREF;
      }
    if (tokens[i].type == '-')
      if (i == 0 || tokens[i - 1].type == '+' || tokens[i - 1].type == '-' ||
          tokens[i - 1].type == '*' || tokens[i - 1].type == '/' ||
          tokens[i - 1].type == '(' || tokens[i - 1].type == TK_EQ ||
          tokens[i - 1].type == TK_NEQ || tokens[i - 1].type == TK_AND ||
          tokens[i - 1].type == TK_DEREF ||
          tokens[i - 1].type == TK_MINUS_ONE) {
        tokens[i].type = TK_MINUS_ONE;
      }
  }

  return true;
}

static int find_parentheses(int p, bool *const success) {
  if (tokens[p].type != '(') {
    *success = false;
    Log("brackets does not match");
    return -1;
  }

  int cnt = 1;
  p++;
  while (p < nr_token && cnt != 0) {
    if (tokens[p].type == '(')
      cnt++;
    else if (tokens[p].type == ')')
      cnt--;
    p++;
  }
  if (p == nr_token && cnt != 0) {
    *success = false;
    Log("brackets does not match");
    return -1;
  }
  return p - 1;
}

static bool check_parentheses(const int p, const int q, bool *const success) {
  if (tokens[p].type != '(' || tokens[q].type != ')') return false;
  int q_match = find_parentheses(p, success);
  if (!(*success)) return false;

  if (q_match == q)
    return true;
  else if (q_match < q)
    return false;
  else {
    *success = false;
    Log("brackets does not match");
    return false;
  }
}

static int operator_precedence[300];

static bool compare_operator_precedence(int operator1, int operator2) {
  operator_precedence['*'] = 3;
  operator_precedence['/'] = 3;
  operator_precedence['+'] = 4;
  operator_precedence['-'] = 4;
  operator_precedence[TK_EQ] = 7;
  operator_precedence[TK_NEQ] = 7;
  operator_precedence[TK_AND] = 11;
  operator_precedence[TK_DEREF] = 2;
  operator_precedence[TK_MINUS_ONE] = 2;
  return operator_precedence[operator1] <= operator_precedence[operator2];
}

static int find_main_op(int p, const int q, bool *const success) {
  int main_op = -1;
  while (p <= q) {
    switch (tokens[p].type) {
      case '+':
      case '-':
      case '*':
      case '/':
      case TK_EQ:
      case TK_NEQ:
      case TK_AND:
      case TK_DEREF:
      case TK_MINUS_ONE:
        if (main_op == -1)
          main_op = p;
        else if (compare_operator_precedence(tokens[main_op].type,
                                             tokens[p].type) &&
                 !((tokens[main_op].type == TK_DEREF ||
                    tokens[main_op].type == TK_MINUS_ONE) &&
                   (tokens[p].type == TK_DEREF ||
                    tokens[p].type == TK_MINUS_ONE)))
          main_op = p;
        p++;
        break;
      case '(':
        p = find_parentheses(p, success) + 1;
        if (!(*success)) return -1;
        break;
      default:
        p++;
        break;
    }
  }
  return main_op;
}

static word_t eval(const int p, const int q, bool *const success) {
  if (p > q) {
    *success = false;
    Log("expression split failed");
    return -1;

  } else if (p == q) {
    word_t number;
    switch (tokens[p].type) {
      case TK_NUMBER:
        if (sscanf(tokens[p].str, FMT_WORD_T, &number) == 1) {
          return number;
        } else {
          *success = false;
          Log("the number in expression is illegal");
          return -1;
        }
        break;
      case TK_NUMBER_HEX:
        if (sscanf(tokens[p].str, FMT_WORD, &number) == 1) {
          return number;
        } else {
          *success = false;
          Log("the number in expression is illegal");
          return -1;
        }
        break;
      case TK_REG:
        bool success_tmp = true;
        number = isa_reg_str2val(tokens[p].str + 1, &success_tmp);
        if (!success_tmp) {
          *success = false;
          Log("the register does not exist");
          return -1;
        } else {
          return number;
        }
        break;
      default:
        *success = false;
        Log("expression split failedl");
        return -1;
        break;
    }

  } else {
    bool check = check_parentheses(p, q, success);
    if (!(*success)) return -1;
    if (check) {
      word_t val = eval(p + 1, q - 1, success);
      if (!(*success)) return -1;
      return val;

    } else {
      int main_op = find_main_op(p, q, success);
      if (!(*success)) return -1;
      word_t val1 = 0;
      if (tokens[main_op].type != TK_DEREF &&
          tokens[main_op].type != TK_MINUS_ONE) {
        val1 = eval(p, main_op - 1, success);
        if (!(*success)) return -1;
      }
      word_t val2 = eval(main_op + 1, q, success);
      if (!(*success)) return -1;
      if (tokens[main_op].type == '/' && val2 == 0) {
        *success = false;
        Log("div 0");
        return -1;
      }
      switch (tokens[main_op].type) {
        case '+':
          return val1 + val2;
        case '-':
          return val1 - val2;
        case '*':
          return val1 * val2;
        case '/':
          return val1 / val2;
        case TK_EQ:
          return val1 == val2;
        case TK_NEQ:
          return val1 != val2;
        case TK_AND:
          return val1 && val2;
        case TK_DEREF:
          return vaddr_read(val2, sizeof(word_t));
        case TK_MINUS_ONE:
          return -val2;
        default:
          *success = false;
          Log("operator analysis failed");
          return -1;
      }
    }
  }
}

word_t expr(const char *const e, bool *const success) {
  if (!make_token(e)) {
    *success = false;
    return -1;
  }

  word_t val = eval(0, nr_token - 1, success);
  if (!(*success)) return -1;

  return val;
}

void test_expr() {
  bool success = true;
  word_t val = expr("  **0x80000010", &success);
  Assert(success, "expression is illegal");
  printf(FMT_WORD_T "\n", val);
}

static char buf[TOKENS_MAX];

void test_expr_auto() {
  FILE *fp =
      fopen("/home/mwxhaha/ysyx-workbench/nemu/tools/gen-expr/input", "r");
  Assert(fp, "file does not exist");
  while (1) {
    word_t result;
    int ret = fscanf(fp, FMT_WORD_T "\n", &result);
    if (ret == EOF) break;
    Assert(ret == 1, "read file error");
    char *ret2 = fgets(buf, TOKENS_MAX, fp);
    buf[strlen(buf) - 1] = '\0';
    Assert(ret2, "read file error");

    bool success = true;
    word_t val = expr(buf, &success);
    Assert(success, "expression is illegal");
    printf(FMT_WORD_T "\n%s\n" FMT_WORD_T "\n", result, buf, val);
    Assert(result == val, "wrong answer");
  }
  fclose(fp);
}