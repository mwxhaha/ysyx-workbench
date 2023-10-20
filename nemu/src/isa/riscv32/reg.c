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

#include "local-include/reg.h"

#include <common.h>
#include <isa.h>
#include <string.h>

const char *regs[] = {"$0", "ra", "sp",  "gp",  "tp", "t0", "t1", "t2",
                      "s0", "s1", "a0",  "a1",  "a2", "a3", "a4", "a5",
                      "a6", "a7", "s2",  "s3",  "s4", "s5", "s6", "s7",
                      "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};
// plan todo
void isa_reg_display() {
  int column_cnt_display = 8;
  for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
    printf("%s:" FMT_WORD " ", regs[i], cpu.gpr[i]);
    if (i % column_cnt_display == column_cnt_display - 1) printf("\n");
  }
  if (sizeof(regs) / sizeof(regs[0]) % column_cnt_display != 0) printf("\n");
  printf("%s:" FMT_WORD "\n", "pc", cpu.pc);
}

word_t isa_reg_str2val(const char *s, bool *success) {
  if (strcmp(s, "pc") == 0) return cpu.pc;
  for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
    if (strcmp(s, regs[i]) == 0) return cpu.gpr[i];
  }
  *success = false;
  return 0;
}
