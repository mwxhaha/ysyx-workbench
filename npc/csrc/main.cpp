#include <util/sim_tool.hpp>
#ifdef SIM_ALL
#include <sim/cpu.hpp>
#endif

void sim();

int main(int argc, char **argv)
{
    sim_init(argc, argv);

    sim();

    sim_exit();

#ifdef SIM_ALL
    return (npc_state.state == NPC_END && npc_state.halt_ret == 0) || npc_state.state == NPC_QUIT ? 0 : 1;
#else
    return 0;
#endif
}
