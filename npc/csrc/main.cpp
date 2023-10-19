#include <util/sim_tool.hpp>
#ifdef SIM_ALL
#include <sim/cpu_sim.hpp>
#endif

void sim();

int main(int argc, char **argv)
{
    sim_init(argc, argv);

    sim();

    sim_exit();

#ifdef SIM_ALL
    return (npc_state.state == npc_end && npc_state.ret == 0) || npc_state.state == npc_quit ? 0 : 1;
#else
    return 0;
#endif
}
