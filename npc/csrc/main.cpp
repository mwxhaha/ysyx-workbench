#include "sim_tool.hpp"

int sim();

int main(int argc, char **argv)
{
    sim_init(argc, argv);

    int ret = sim();

    sim_exit();

    return ret;
}
