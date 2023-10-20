#include <cpu_init.hpp>

#include <cstdbool>
#include <cstring>

#include <iostream>
#include <fstream>

#include <sim/cpu_sim.hpp>
#include <util/disasm.hpp>
#include <util/debug.hpp>
#include <sdb/cpu_sdb.hpp>
#include <sdb/cpu_ftrace.hpp>

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
#ifdef CONFIG_RV64
    panic("do not support");
#else
    init_disasm("riscv32-pc-linux-gnu");
#endif
#endif
    argc = argc_tmp;
}