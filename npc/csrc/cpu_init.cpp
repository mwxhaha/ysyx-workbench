#include <cpu_init.hpp>
#include <sim_tool.hpp>
#include <iostream>
#include <fstream>
#include <cpu_disasm.hpp>

static void load_img(int &argc, char **argv)
{
    for (int i = 0; i < argc; i++)
        if (strcmp(argv[i], "-i") == 0)
        {
            const char *img_file = argv[2];
            std::cout << "use image" << img_file << std::endl;
            std::ifstream fin;
            fin.open(img_file, std::ios::in);
            fin.read(reinterpret_cast<char *>(mem), MEM_MAX);
            fin.close();
            argc -= 2;
            return;
        }
}

void init(int &argc, char **argv)
{
#ifdef CONFIG_ITRACE
    init_disasm("riscv32-pc-linux-gnu");
#endif
    load_img(argc, argv);
}