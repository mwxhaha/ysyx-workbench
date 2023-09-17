/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string.h>
#include <debug.h>
#include <stdbool.h>
#include <common.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUMBER

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"-", '-'},         
  {"\\*", '*'},         
  {"/", '/'},     
  {"\\(", '('},     
  {"\\)", ')'},     
  {"[0-9]+", TK_NUMBER}
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        Assert(strlen(substr_start) <= 31, "number is too large");
        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case TK_NUMBER:
            Assert(memcpy(tokens[nr_token].str, substr_start, substr_len),"string process error");
            tokens[nr_token].str[substr_len] = '\0';
          default:
            tokens[nr_token].type = rules[i].token_type;
            nr_token++;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      Log("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

static int find_parentheses(int p, bool *success) {
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

static bool check_parentheses(int p, int q, bool *success) {
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

static word_t eval(int p, int q, bool *success) {
  if (p > q) {
    *success = false;
    Log("expression split failed");
    return 2147483647;

  } else if (p == q) {
    if (tokens[p].type != TK_NUMBER) {
      *success = false;
      Log("the number in expression is illegal");
      return 2147483647;
    }
    word_t number;
    if (sscanf(tokens[p].str, "%d", &number) == 1) {
      return number;
    } else {
      *success = false;
      Log("the number in expression is illegal");
      return 2147483647;
    }

  } else {
    bool check = check_parentheses(p, q, success);
    if (!(*success)) return 2147483647;
    if (check) {
      /* The expression is surrounded by a matched pair of parentheses.
       * If that is the case, just throw away the parentheses.
       */
      word_t val = eval(p + 1, q - 1, success);
      if (!(*success)) return 2147483647;
      return val;
    } else {

      int main_op = -1;
      int i = p;
      while (i <= q) {
        switch (tokens[i].type) {
          case '+':
          case '-':
            if (main_op == -1 || tokens[main_op].type == '+' ||
                tokens[main_op].type == '-' || tokens[main_op].type == '*' ||
                tokens[main_op].type == '/')
              main_op = i;
            i++;
            break;
          case '*':
          case '/':
            if (main_op == -1 || tokens[main_op].type == '*' ||
                tokens[main_op].type == '/')
              main_op = i;
            i++;
            break;
          case '(':
            i = find_parentheses(i, success) + 1;
            if (!(*success)) return 2147483647;
            break;
          default:
            i++;
            break;
        }
      }
      word_t val1 = eval(p, main_op - 1, success);
      if (!(*success)) return 2147483647;
      word_t val2 = eval(main_op + 1, q, success);
      if (!(*success)) return 2147483647;
      if (tokens[main_op].type == '/' && val2 == 0) {
        *success = false;
        Log("div 0");
        return 2147483647;
      }
      switch (tokens[main_op].type) {
        case '+':
          return val1 + val2;
        case '-': /* ... */
          return val1 - val2;
        case '*': /* ... */
          return val1 * val2;
        case '/': /* ... */
          return val1 / val2;
        default:
          *success = false;
          Log("operator analysis failed");
          return 2147483647;
      }
    }
  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 2147483647;
  }

  /* TODO: Insert codes to evaluate the expression. */
  word_t val = eval(0, nr_token - 1, success);
  if (!(*success)) return 2147483647;

  return val;
}
