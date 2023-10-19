#include <cpu_init.hpp>
#include <util/sim_tool.hpp>
#include <sim/cpu_sim.hpp>
#include <iostream>
#include <fstream>
#include <util/disasm.hpp>
#include <sdb/cpu_sdb.hpp>
#include <sdb/cpu_ftrace.hpp>
#include <cstdbool>

static bool load_img(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
        if (strcmp(argv[i], "-i") == 0)
        {
            const char *img_file = argv[i + 1];
            std::cout << "use image: " << img_file << std::endl;
            std::ifstream fin;
            fin.open(img_file, std::ios::in);
            fin.read(reinterpret_cast<char *>(mem), MEM_MAX);
            fin.close();
            argc -= 2;
            return true;
        }
    return false;
}

void init(int &argc, char **argv)
{
    int argc_tmp = argc;
    if (load_img(argc, argv))
        argc_tmp -= 2;
    if (load_elf(argc, argv))
        argc_tmp -= 2;
    init_sdb();
#ifdef CONFIG_ITRACE
    init_disasm("riscv32-pc-linux-gnu");
#endif
    argc = argc_tmp;
}