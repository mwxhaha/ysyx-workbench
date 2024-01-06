#include <cpu/cpu_mem.hpp>

#include <cstdint>

#include <sim/cpu_sim.hpp>
#include <util/debug.hpp>

uint8_t mem[MEM_MAX] = {0x97, 0x14, 0x00, 0x00,  // auipc 9 4096
                        0xb3, 0x86, 0xb4, 0x00,  // add 13 9 11
                        0xa3, 0xaf, 0x96, 0xfe,  // sw 9 -1(13)
                        0x83, 0xa5, 0xf4, 0xff,  // lw 11 -1(9)
                        0x63, 0x84, 0xb6, 0x00,  // beq 11 13 4
                        0xef, 0x04, 0x40, 0x00,  // jal 9 4
                        0x73, 0x00, 0x10, 0x00}; // ebreak

void pmem_read(vaddr_t raddr, word_t *rdata)
{
#if (ISA_WIDTH == 64)
    panic("do not isa64");
#else
    Assert(raddr >= MEM_BASE_ADDR, "memory out of bound");
    Assert(raddr <= MEM_BASE_ADDR + MEM_MAX - 1, "memory out of bound");
    word_t *addr_real = (word_t *)raddr - (word_t *)MEM_BASE_ADDR + (word_t *)mem;
    *rdata = *addr_real;
#endif
}

void pmem_write(vaddr_t waddr, word_t wdata, uint8_t wmask)
{
#if (ISA_WIDTH == 64)
    panic("do not isa64");
#else
    Assert(waddr >= MEM_BASE_ADDR, "memory out of bound");
    Assert(waddr <= MEM_BASE_ADDR + MEM_MAX - 1, "memory out of bound");
    word_t *addr_real = (word_t *)waddr - (word_t *)MEM_BASE_ADDR + (word_t *)mem;
    switch (wmask)
    {
    case 1:
        *(uint8_t *)addr_real = wdata;
        break;
    case 3:
        *(uint16_t *)addr_real = wdata;
        break;
    case 15:
        *(uint32_t *)addr_real = wdata;
        break;
    default:
        panic("memory write mask error");
        break;
    }
#endif
}
