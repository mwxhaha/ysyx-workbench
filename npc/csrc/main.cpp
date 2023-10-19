#include <sim_tool.hpp>

void sim();

int main(int argc, char **argv)
{
    sim_init(argc, argv);

    sim();

    sim_exit();

#ifdef SIM_ALL
    pin_output(npc_state.pc, 32, 0, 1, 0, 0);
    return (npc_state.state == npc_end && npc_state.ret == 0) ? 0 : 1;
#else
    return 0;
#endif
}
