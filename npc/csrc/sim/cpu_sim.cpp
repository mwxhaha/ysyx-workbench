#include <sim/cpu_sim.hpp>

#include <util/sim_tool.hpp>
#include <monitor/cpu_sdb.hpp>
#include <util/debug.hpp>

uint8_t mem[MEM_MAX] = {0x97, 0x14, 0x00, 0x00,  // auipc 9 4096
                        0xb3, 0x86, 0xb4, 0x00,  // add 13 9 11
                        0xa3, 0xaf, 0x96, 0xfe,  // sw 9 -1(13)
                        0x83, 0xa5, 0xf4, 0xff,  // lw 11 -1(9)
                        0x63, 0x84, 0xb6, 0x00,  // beq 11 13 4
                        0xef, 0x04, 0x40, 0x00,  // jal 9 4
                        0x73, 0x00, 0x10, 0x00}; // ebreak
npc_state_t npc_state = {1, npc_stop, MEM_BASE_ADDR};

void sim()
{
#ifdef NV_SIM
    panic("do not support nvboard");
#else
    reset();
    sdb_mainloop();
#endif
}
