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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "sdb.h"

#define NR_WP 32
#define WP_EXPR_MAX 32

typedef struct watchpoint {
  bool used;
  char e[WP_EXPR_MAX + 1];
  word_t val;
} WP;

static WP wp_pool[NR_WP] = {};

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i++) {
    wp_pool[i].used = false;
    wp_pool[i].e[0] = '\0';
  }
}

int new_wp(const char *const e) {
  int len = strlen(e);
  if (len >= WP_EXPR_MAX) {
    Log("expression is too long");
    return 1;
  }
  int i;
  for (i = 0; i < NR_WP; i++) {
    if (!wp_pool[i].used) {
      wp_pool[i].used = true;
      strcpy(wp_pool[i].e, e);
      bool success = true;
      wp_pool[i].val = expr(wp_pool[i].e, &success);
      if (!success) {
        Log("expression is illegal, can not set the watchpoint");
        wp_pool[i].used = false;
        return 1;
      }
      printf("successfully set the watchpoint, N: %d, expr: %s\n", i,
             wp_pool[i].e);
      return 0;
    }
  }
  Log("there is no free watchpoint");
  return 1;
}

int free_wp(const int n) {
  if (n <= -1 || n >= NR_WP) {
    Log("the N is out of range");
    return 1;
  }
  if (!wp_pool[n].used) {
    Log("the watchpoint is orignally free");
    return 1;
  }
  printf("successfully delete the watchpoint, N: %d, expr: %s\n", n,
         wp_pool[n].e);
  wp_pool[n].used = false;
  wp_pool[n].e[0] = '\0';
  return 0;
}

bool check_watchpoint() {
  bool stop_flag = false;
  for (int i = 0; i < NR_WP; i++)
    if (wp_pool[i].used) {
      bool success = true;
      word_t new_val = expr(wp_pool[i].e, &success);
      if (!success) panic("The expression was illegally modified");
      if (wp_pool[i].val != new_val) {
        Log("watchpoint changes, N: %d, expr: %s, value: " FMT_WORD
            " -> " FMT_WORD,
            i, wp_pool[i].e, wp_pool[i].val, new_val);
        stop_flag = true;
      }
      wp_pool[i].val = new_val;
    }
  return stop_flag;
}

void printf_watchpoint() {
  printf("watchpoint                             expr      value\n");
  bool empty_flag = true;
  for (int i = 0; i < NR_WP; i++)
    if (wp_pool[i].used) {
      printf("%10d %32s " FMT_WORD "\n", i, wp_pool[i].e, wp_pool[i].val);
      empty_flag = false;
    }
  if (empty_flag) printf("watchpoint pool is empty\n");
}

// typedef struct watchpoint {
//   int NO;
//   struct watchpoint *prev;
//   struct watchpoint *next;
//   char e[WP_EXPR_MAX + 1];
// } WP;

// static WP wp_pool[NR_WP] = {};
// static WP *wp_head = NULL, *wp_tail = NULL, *free_head = wp_pool,
//           *free_tail = &wp_pool[NR_WP - 1];

// void init_wp_pool() {
//   int i;
//   for (i = 0; i < NR_WP; i++) {
//     wp_pool[i].NO = i;
//     wp_pool[i].prev = (i == 0 ? NULL : &wp_pool[i - 1]);
//     wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
//   }
// }

// static WP *new_wp() {
//   if (wp_head == NULL && wp_tail == NULL) {
//     wp_head = free_head;
//     wp_tail = free_head;
//     free_head = free_head->next;
//     wp_tail->next = NULL;
//     free_head->prev = NULL;
//   } else {
//     if (free_head == NULL && free_tail == NULL) {
//       Log("there is no free watchpoint");
//       return NULL;
//     } else if (free_head != NULL && free_head == free_tail) {
//       wp_tail->next = free_head;
//       free_head->prev = wp_tail;
//       wp_tail = free_head;
//       free_head = NULL;
//       free_tail = NULL;
//     } else {
//       wp_tail->next = free_head;
//       free_head->prev = wp_tail;
//       wp_tail = free_head;
//       free_head = free_head->next;
//       wp_tail->next = NULL;
//       free_head->prev = NULL;
//     }
//   }
//   return wp_tail;
// }

// static int free_wp(WP *wp) {
//   if (free_head == NULL && free_tail == NULL) {
//     wp->prev->next = wp->next;
//     wp->next->prev = wp->prev;
//     wp->prev = NULL;
//     wp->next = NULL;
//     free_head = wp;
//     free_tail = wp;
//   } else {
//     if (wp_head == NULL && wp_tail == NULL) {
//       Log("there is no used watchpoint");
//       return 1;
//     } else if (wp_head != NULL && wp_head == wp_tail) {
//       if (wp != wp_head) {
//         Log("this watchpoint is originally empty");
//         return 1;
//       }
//       wp_head = NULL;
//       wp_tail = NULL;
//       free_tail->next = wp;
//       wp->prev = free_tail;
//       free_tail = wp;
//     } else if (wp == wp_head) {
//       wp_head->next->prev = NULL;
//       wp_head = wp_head->next;
//       free_tail->next = wp;
//       wp->prev = free_tail;
//       wp->next = NULL;
//       free_tail = wp;
//     } else if (wp == wp_tail) {
//       wp_tail->prev->next = NULL;
//       wp_tail = wp_tail->prev;
//       free_tail->next = wp;
//       wp->prev = free_tail;
//       wp->next = NULL;
//       free_tail = wp;
//     } else {
//       WP *this_wp_tail = wp;
//       while (this_wp_tail != free_tail) {
//         this_wp_tail = this_wp_tail->next;
//         if (this_wp_tail == wp_tail) {
//           Log("this watchpoint is originally empty");
//           return 1;
//         }
//       }
//       wp->prev->next = wp->next;
//       wp->next->prev = wp->prev;
//       free_tail->next = wp;
//       wp->prev = free_tail;
//       wp->next = NULL;
//       free_tail = wp;
//     }
//   }
//   return 0;
// }

// int set_watchpoint(const char *e) {
//   int len = strlen(e);
//   if (len >= WP_EXPR_MAX) {
//     Log("expression is too long");
//     return 1;
//   }
//   WP *wp = new_wp();
//   if (wp == NULL) return 1;
//   strcpy(wp->e, e);
//   return 0;
// }

// int delete_watchpoint(int n) {
//   if (n <= -1 || n >= NR_WP) {
//     Log("the NO is out of range");
//     return 1;
//   }
//   if (free_wp(&wp_pool[n]) != 0) return 1;
//   return 0;
// }
