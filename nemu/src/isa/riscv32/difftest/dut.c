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

#include <cpu/difftest.h>
#include <isa.h>
#include <stdbool.h>
#include <stdio.h>

#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc)
{
    bool ret = true;
    for (int i = 0; i < GPR_NUM; i++)
        if (cpu.gpr[i] != ref_r->gpr[i])
        {
            printf("reg %s: " FMT_WORD " is different, it should be " FMT_WORD "\n", regs[i],
                   cpu.gpr[i], ref_r->gpr[i]);
            ret = false;
        }
    if (!ret)
        printf("In pc: " FMT_WORD "\n", pc);
    return ret;
}

void isa_difftest_attach() {}
