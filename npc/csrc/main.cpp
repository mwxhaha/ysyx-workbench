#include "sim_tool.hpp"

void sim();

int main(int argc, char **argv)
{
    sim_init(argc, argv);

    sim();

    sim_exit();

    return (npc_state.state==end&&npc_state.ret==0)?0:1;
}
