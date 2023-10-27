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

#include <memory/host.h>
#include <common.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>
#include <stdbool.h>

#if defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
#endif

uint8_t *guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }
static bool enable_mtrace = true;

void disable_mtrace_once()
{
	enable_mtrace = false;
}

static word_t pmem_read(paddr_t addr, int len)
{
	word_t ret = host_read(guest_to_host(addr), len);
#ifdef CONFIG_MTRACE // plan todo
	if (enable_mtrace)
	{
		printf("memory read in addr " FMT_WORD " with len %d: " FMT_WORD "\n", addr, len, ret);
	}
	enable_mtrace = true;
#endif
	return ret;
}

static void pmem_write(paddr_t addr, int len, word_t data)
{
#ifdef CONFIG_MTRACE // plan todo
	word_t write_data_before, write_data_after;
	if (enable_mtrace)
	{
		write_data_before = host_read(guest_to_host(addr), len);
	}
#endif
	host_write(guest_to_host(addr), len, data);
#ifdef CONFIG_MTRACE // plan todo
	if (enable_mtrace)
	{
		write_data_after = host_read(guest_to_host(addr), len);
		printf("memory write in addr " FMT_WORD " with len %d: " FMT_WORD "->" FMT_WORD "\n", addr, len, write_data_before, write_data_after);
	}
	enable_mtrace = true;
#endif
}

static void out_of_bound(paddr_t addr)
{
	panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
		  addr, PMEM_LEFT, PMEM_RIGHT, cpu.pc);
}

void init_mem()
{
#if defined(CONFIG_PMEM_MALLOC)
	pmem = malloc(CONFIG_MSIZE);
	assert(pmem);
#endif
#ifdef CONFIG_MEM_RANDOM
	uint32_t *p = (uint32_t *)pmem;
	int i;
	for (i = 0; i < (int)(CONFIG_MSIZE / sizeof(p[0])); i++)
	{
		p[i] = rand();
	}
#endif
#ifdef CONFIG_TARGET_NATIVE_ELF
	Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
#endif
}

void mem_quit()
{
#if defined(CONFIG_PMEM_MALLOC)
	free(pmem);
#endif
}

word_t paddr_read(paddr_t addr, int len)
{
	if (likely(in_pmem(addr)))
		return pmem_read(addr, len);
	IFDEF(CONFIG_DEVICE, return mmio_read(addr, len));
	out_of_bound(addr);
	return 0;
}

void paddr_write(paddr_t addr, int len, word_t data)
{
	if (likely(in_pmem(addr)))
	{
		pmem_write(addr, len, data);
		return;
	}
	IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
	out_of_bound(addr);
}
