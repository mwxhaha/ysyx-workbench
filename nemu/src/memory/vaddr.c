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
#include <device/map.h>
#include <stdbool.h>

static word_t vaddr_ifetch(vaddr_t addr, int len)
{
    return paddr_read(addr, len);
}

static word_t vaddr_read(vaddr_t addr, int len)
{
    return paddr_read(addr, len);
}

static void vaddr_write(vaddr_t addr, int len, word_t data)
{
    paddr_write(addr, len, data);
}

word_t addr_montior_read(vaddr_t addr, int len)
{
    enable_mem_align_check = false;
    enable_mtrace = false;
    enable_dtrace = false;
    enable_device_fresh = false;
    enable_device_skip_diff = false;
    word_t ret = vaddr_read(addr, len);
    enable_mem_align_check = true;
    enable_mtrace = true;
    enable_dtrace = true;
    enable_device_fresh = true;
    enable_device_skip_diff = true;
    return ret;
}

word_t addr_ifetch(vaddr_t addr, int len)
{
    enable_mtrace = false;
    enable_dtrace = false;
    enable_device_fresh = false;
    enable_device_skip_diff = false;
    word_t ret = vaddr_ifetch(addr, len);
    enable_mtrace = true;
    enable_dtrace = true;
    enable_device_fresh = true;
    enable_device_skip_diff = true;
    return ret;
}

word_t addr_read(vaddr_t addr, int len)
{
    return vaddr_read(addr, len);
}

void addr_write(vaddr_t addr, int len, word_t data)
{
    vaddr_write(addr, len, data);
}
