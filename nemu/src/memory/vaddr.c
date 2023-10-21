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
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len) {
#ifdef CONFIG_MTRACE
  word_t read_data = paddr_read(addr, len);
  printf("memory read in addr " FMT_PADDR " with len %d: " FMT_WORD "\n", addr, len, read_data);
  return read_data ;
#else
  return paddr_read(addr, len);
#endif
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
#ifdef CONFIG_MTRACE
  word_t write_data_before  = paddr_read(addr, len);
  paddr_write(addr, len, data);
  word_t write_data_after  = paddr_read(addr, len);
  printf("memory write in addr " FMT_PADDR " with len %d: " FMT_WORD "->" FMT_WORD "\n", addr, len, write_data_before, write_data_after);
#else
  paddr_write(addr, len, data);
#endif
}
