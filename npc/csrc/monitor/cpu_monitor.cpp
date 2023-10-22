#include <monitor/cpu_monitor.hpp>

#include <cstring>
#include <cstdio>
#include <getopt.h>
#include <cstddef>
#include <cstdlib>

#include <verilated.h>
#include <sim/cpu_sim.hpp>
#include <util/disasm.hpp>
#include <util/sim_tool.hpp>
#include <util/debug.hpp>
#include <monitor/cpu_sdb.hpp>
#include <cpu/cpu_ftrace.hpp>
#include <cpu/cpu_dut.hpp>

static char *diff_so_file = NULL;
static char *img_file = NULL;
static char *elf_file = NULL;
static char *verilator_argv = NULL;

static long load_img()
{
    if (img_file == NULL)
    {
        printf("No image is given. Use the default build-in image.\n");
        return 4096; // built-in image size
    }

    FILE *fp = fopen(img_file, "rb");
    Assert(fp, "Can not open '%s'", img_file);

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);

    printf("The image is %s, size = %ld\n", img_file, size);

    fseek(fp, 0, SEEK_SET);
    int ret = fread(mem, size, 1, fp);
    assert(ret == 1);

    fclose(fp);
    return size;
}

static int parse_args(int argc, char *argv[])
{
    const struct option table[] = {
        {"batch", no_argument, NULL, 'b'},
        {"diff", required_argument, NULL, 'd'},
        {"elf", required_argument, NULL, 'e'},
        {"verilator", required_argument, NULL, 'v'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, NULL, 0},
    };
    int o;
    while ((o = getopt_long(argc, argv, "-bhd:e:v:", table, NULL)) != -1)
    {
        switch (o)
        {
        case 'b':
            sdb_set_batch_mode();
            break;
        case 'd':
            diff_so_file = optarg;
            break;
        case 'e':
            elf_file = optarg;
            break;
        case 'v':
            verilator_argv = optarg;
            break;
        case 1:
            img_file = optarg;
            return 0;
        default:
            printf("Usage: %s [OPTION...] IMAGE [args]\n\n", argv[0]);
            printf("\t-b,--batch              run with batch mode\n");
            printf("\t-e,--elf=FILE           elf FILE for trace\n");
            printf("\t-v,--verilator=\"verilator argv\"      verilator argv\n");
            printf("\t-d,--diff=REF_SO        run DiffTest with reference REF_SO\n");
            printf("\n");
            exit(0);
        }
    }
    return 0;
}

#define VERILATOR_ARGV_MAX 10

static void init_verilator(char *argv0)
{
    if (verilator_argv == NULL)
    {
        printf("No verilator argv is given\n");
        return;
    }
    int argc = 1;
    char *argv[VERILATOR_ARGV_MAX];
    argv[0] = argv0;
    argv[1] = strtok(verilator_argv, " ");
    while (argv[argc])
    {
        argc++;
        Assert(argc < VERILATOR_ARGV_MAX, "too many verilator argv");
        argv[argc] = strtok(NULL, " ");
    }
    contextp->commandArgs(argc, argv);
}

void init_monitor(int argc, char *argv[])
{
    parse_args(argc, argv);

    long img_size = load_img();

    load_elf(elf_file);

    /* Initialize differential testing. */
#ifdef CONFIG_DIFFTEST
    init_difftest(diff_so_file, img_size);
#endif

    /* Initialize the simple debugger. */
    init_sdb();

    init_verilator(argv[0]);

#ifdef CONFIG_ITRACE
#if CONFIG_ISA == CONFIG_RV32I
    init_disasm("riscv32-pc-linux-gnu");
#elif CONFIG_ISA == CONFIG_RV32E
    init_disasm("riscv32-pc-linux-gnu");
#elif CONFIG_ISA == CONFIG_RV64I
    init_disasm("riscv64-pc-linux-gnu");
#else
#error "do not support ISA " #CONFIG_ISA
#endif
#endif
}
