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
#include <common.h>
#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <difftest-def.h>
#include <memory/paddr.h>

#include <stdio.h>

__EXPORT void difftest_memcpy(vaddr_t addr, void *buf, size_t n, bool direction)
{ // plan todo
    if (direction == DIFFTEST_TO_DUT)
    {
        for (size_t i = 0; i < n; i++)
        {
            ((uint8_t *)buf)[addr + i] = paddr_read(addr + i, 1);
        }
    }
    else if (direction == DIFFTEST_TO_REF)
    {
        for (size_t i = 0; i < n; i++)
        {
            paddr_write(addr + i, 1, ((uint8_t *)buf)[addr + i - CONFIG_MBASE]);
        }
    }
}

__EXPORT void difftest_regcpy(void *dut, bool direction)
{
    if (direction == DIFFTEST_TO_DUT)
    {
        (*((CPU_state *)dut)).pc = cpu.pc;
        for (int i = 0; i < RISCV_GPR_NUM; i++)
        {
            (*((CPU_state *)dut)).gpr[i] = cpu.gpr[i];
        }
    }
    else if (direction == DIFFTEST_TO_REF)
    {
        cpu.pc = (*((CPU_state *)dut)).pc;
        for (int i = 0; i < RISCV_GPR_NUM; i++)
        {
            cpu.gpr[i] = (*((CPU_state *)dut)).gpr[i];
        }
    }
}

__EXPORT void difftest_exec(uint64_t n)
{
    Decode s0;
    Decode *s = &s0;
    s->pc = cpu.pc;
    s->snpc = cpu.pc;
    isa_exec_once(s);
    cpu.pc = s->dnpc;
}

__EXPORT void difftest_raise_intr(word_t NO)
{
    assert(0);
}

__EXPORT void difftest_init()
{
    void init_mem();
    init_mem();
    /* Perform ISA dependent initialization. */
    init_isa();
}
